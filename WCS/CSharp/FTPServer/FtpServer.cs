using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace FTPServer
{
    /// <summary>
    /// C++의 FTPServer(6.0)에 대응하는 간이 FTP 서버.
    /// ECS 로그 파일 수집을 위한 기본 FTP 기능(LIST, RETR, STOR)을 제공한다.
    /// RFC 959 기반, Passive Mode(PASV) 지원.
    /// </summary>
    public class EcsFtpServer
    {
        private TcpListener? _listener;
        private CancellationTokenSource _cts = new();
        private readonly ConcurrentDictionary<string, FtpSession> _sessions = new();

        public int    Port     { get; set; } = 21;
        public string RootPath { get; set; } = @"C:\EcsFtp";
        public bool   Running  { get; private set; }

        public event EventHandler<string>? Log;

        public void Start()
        {
            if (Running) return;
            Directory.CreateDirectory(RootPath);
            _cts     = new CancellationTokenSource();
            _listener= new TcpListener(IPAddress.Any, Port);
            _listener.Start();
            Running  = true;
            Task.Run(() => AcceptLoop(_cts.Token));
            OnLog($"FTP 서버 시작 포트:{Port} 루트:{RootPath}");
        }

        public void Stop()
        {
            if (!Running) return;
            Running = false;
            _cts.Cancel();
            _listener?.Stop();
            OnLog("FTP 서버 중지");
        }

        private async Task AcceptLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested && _listener != null)
            {
                try
                {
                    var client = await _listener.AcceptTcpClientAsync();
                    var session = new FtpSession(client, RootPath, OnLog);
                    string id = Guid.NewGuid().ToString("N");
                    _sessions[id] = session;
                    _ = session.RunAsync(token).ContinueWith(t => { _sessions.TryRemove(id, out FtpSession removed); });
                }
                catch (ObjectDisposedException) { break; }
                catch { }
            }
        }

        private void OnLog(string msg) => Log?.Invoke(this, msg);
    }

    /// <summary>FTP 세션 핸들러 - 클라이언트 1개 연결에 대응.</summary>
    internal class FtpSession
    {
        private readonly TcpClient _ctrl;
        private readonly string    _root;
        private readonly Action<string> _log;
        private string _user    = string.Empty;
        private string _curDir  = "/";
        private bool   _loggedIn;
        private TcpListener? _pasvListener;
        private string _renameFrom = string.Empty;

        public FtpSession(TcpClient ctrl, string root, Action<string> log)
        {
            _ctrl = ctrl; _root = root; _log = log;
        }

        public async Task RunAsync(CancellationToken token)
        {
            using var stream  = _ctrl.GetStream();
            using var reader  = new StreamReader(stream, Encoding.ASCII, true, 4096, true);
            using var writer  = new StreamWriter(stream, Encoding.ASCII, 4096, true) { AutoFlush = true };

            string peer = ((IPEndPoint?)_ctrl.Client.RemoteEndPoint)?.ToString() ?? "unknown";
            _log($"FTP 연결: {peer}");

            await writer.WriteLineAsync("220 ECS FTP Server Ready");

            while (!token.IsCancellationRequested)
            {
                string? line = await reader.ReadLineAsync();
                if (line == null) break;

                int sp = line.IndexOf(' ');
                string cmd  = (sp > 0 ? line.Substring(0, sp) : line).ToUpper().Trim();
                string arg  = sp > 0 ? line.Substring(sp + 1).Trim() : string.Empty;

                await HandleCommand(cmd, arg, writer, token);
            }
            _log($"FTP 연결 종료: {peer}");
        }

        private async Task HandleCommand(string cmd, string arg, StreamWriter w, CancellationToken token)
        {
            switch (cmd)
            {
                case "USER":
                    _user = arg;
                    await w.WriteLineAsync("331 Password required");
                    break;
                case "PASS":
                    _loggedIn = true; // 간이 서버: 모든 패스워드 허용
                    await w.WriteLineAsync($"230 User {_user} logged in");
                    break;
                case "SYST":
                    await w.WriteLineAsync("215 Windows_NT");
                    break;
                case "FEAT":
                    await w.WriteLineAsync("211-Features:");
                    await w.WriteLineAsync(" PASV");
                    await w.WriteLineAsync("211 End");
                    break;
                case "TYPE":
                    await w.WriteLineAsync("200 Type set");
                    break;
                case "PWD":
                    await w.WriteLineAsync($"257 \"{_curDir}\" is current directory");
                    break;
                case "CWD":
                    _curDir = arg.StartsWith("/") ? arg : _curDir.TrimEnd('/') + "/" + arg;
                    await w.WriteLineAsync("250 Directory changed");
                    break;
                case "PASV":
                    await HandlePasv(w);
                    break;
                case "LIST":
                case "NLST":
                    await HandleList(w, token);
                    break;
                case "RETR":
                    await HandleRetr(arg, w, token);
                    break;
                case "STOR":
                    await HandleStor(arg, w, token);
                    break;
                case "DELE":
                    HandleDele(arg, w);
                    break;
                case "MKD":
                    HandleMkd(arg, w);
                    break;
                case "RNFR":
                    _renameFrom = arg;
                    await w.WriteLineAsync("350 Ready for RNTO");
                    break;
                case "RNTO":
                    HandleRnto(arg, w);
                    break;
                case "QUIT":
                    await w.WriteLineAsync("221 Goodbye");
                    break;
                default:
                    await w.WriteLineAsync("502 Command not implemented");
                    break;
            }
        }

        private async Task HandlePasv(StreamWriter w)
        {
            _pasvListener?.Stop();
            _pasvListener = new TcpListener(IPAddress.Any, 0);
            _pasvListener.Start();
            int port = ((IPEndPoint)_pasvListener.LocalEndpoint).Port;
            int p1 = port / 256, p2 = port % 256;
            var ip = "127,0,0,1"; // 실제 배포 시 서버 IP로 교체
            await w.WriteLineAsync($"227 Entering Passive Mode ({ip},{p1},{p2})");
        }

        private async Task HandleList(StreamWriter w, CancellationToken token)
        {
            if (_pasvListener == null) { await w.WriteLineAsync("425 No data connection"); return; }
            await w.WriteLineAsync("150 Opening data connection");

            string physPath = GetPhysPath(_curDir);
            using var dataClient = await _pasvListener.AcceptTcpClientAsync();
            using var dataStream = dataClient.GetStream();
            using var sw = new StreamWriter(dataStream, Encoding.ASCII);

            if (Directory.Exists(physPath))
            {
                foreach (var f in Directory.GetFiles(physPath))
                {
                    var fi = new FileInfo(f);
                    await sw.WriteLineAsync($"-rw-r--r-- 1 ecs ecs {fi.Length,10} {fi.LastWriteTime:MMM dd HH:mm} {fi.Name}");
                }
                foreach (var d in Directory.GetDirectories(physPath))
                    await sw.WriteLineAsync($"drwxr-xr-x 1 ecs ecs          0 {Directory.GetLastWriteTime(d):MMM dd HH:mm} {Path.GetFileName(d)}");
            }
            await sw.FlushAsync();
            await w.WriteLineAsync("226 Transfer complete");
            _pasvListener.Stop(); _pasvListener = null;
        }

        private async Task HandleRetr(string name, StreamWriter w, CancellationToken token)
        {
            if (_pasvListener == null) { await w.WriteLineAsync("425 No data connection"); return; }
            string path = Path.Combine(GetPhysPath(_curDir), name);
            if (!File.Exists(path)) { await w.WriteLineAsync("550 File not found"); return; }

            await w.WriteLineAsync("150 Opening data connection");
            using var dataClient = await _pasvListener.AcceptTcpClientAsync();
            using var dataStream = dataClient.GetStream();
            using var fs = File.OpenRead(path);
            await fs.CopyToAsync(dataStream, 81920, token);
            await w.WriteLineAsync("226 Transfer complete");
            _pasvListener.Stop(); _pasvListener = null;
        }

        private async Task HandleStor(string name, StreamWriter w, CancellationToken token)
        {
            if (_pasvListener == null) { await w.WriteLineAsync("425 No data connection"); return; }
            string path = Path.Combine(GetPhysPath(_curDir), name);

            await w.WriteLineAsync("150 Opening data connection");
            using var dataClient = await _pasvListener.AcceptTcpClientAsync();
            using var dataStream = dataClient.GetStream();
            using var fs = File.Create(path);
            await dataStream.CopyToAsync(fs, 81920, token);
            _log($"STOR: {path}");
            await w.WriteLineAsync("226 Transfer complete");
            _pasvListener.Stop(); _pasvListener = null;
        }

        private async void HandleDele(string name, StreamWriter w)
        {
            string path = Path.Combine(GetPhysPath(_curDir), name);
            try { File.Delete(path); await w.WriteLineAsync("250 Deleted"); }
            catch { await w.WriteLineAsync("550 Delete failed"); }
        }

        private async void HandleMkd(string name, StreamWriter w)
        {
            string path = Path.Combine(GetPhysPath(_curDir), name);
            Directory.CreateDirectory(path);
            await w.WriteLineAsync($"257 \"{name}\" directory created");
        }

        private async void HandleRnto(string to, StreamWriter w)
        {
            if (string.IsNullOrEmpty(_renameFrom)) { await w.WriteLineAsync("503 Bad sequence"); return; }
            string src = Path.Combine(GetPhysPath(_curDir), _renameFrom);
            string dst = Path.Combine(GetPhysPath(_curDir), to);
            try { File.Move(src, dst); await w.WriteLineAsync("250 Rename OK"); }
            catch { await w.WriteLineAsync("550 Rename failed"); }
            _renameFrom = string.Empty;
        }

        private string GetPhysPath(string virtPath)
        {
            string rel = virtPath.TrimStart('/').Replace('/', Path.DirectorySeparatorChar);
            return Path.Combine(_root, rel);
        }
    }
}
