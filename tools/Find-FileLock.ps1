# [LGLS] 잠긴 파일을 어느 프로세스가 쥐고 있는지 찾는다 (재시작 관리자 API)
#   .\Find-FileLock.ps1 "C:\Windows\Fonts\xxx.ttf"
param([Parameter(Mandatory=$true)][string]$Path)

$src = @"
using System;using System.Collections.Generic;using System.Runtime.InteropServices;
public static class RM {
  [StructLayout(LayoutKind.Sequential, Pack=4)] struct FT { public uint lo; public uint hi; }
  [StructLayout(LayoutKind.Sequential, Pack=4)] struct UNIQUE { public uint dwProcessId; public FT ProcessStartTime; }
  [StructLayout(LayoutKind.Sequential, Pack=4, CharSet=CharSet.Unicode)]
  struct INFO { public UNIQUE Process;
    [MarshalAs(UnmanagedType.ByValTStr,SizeConst=256)] public string strAppName;
    [MarshalAs(UnmanagedType.ByValTStr,SizeConst=64)]  public string strServiceShortName;
    public int ApplicationType; public uint AppStatus; public uint TSSessionId;
    [MarshalAs(UnmanagedType.Bool)] public bool bRestartable; }
  [DllImport("rstrtmgr.dll",CharSet=CharSet.Unicode)] static extern int RmStartSession(out uint h,int f,string key);
  [DllImport("rstrtmgr.dll")] static extern int RmEndSession(uint h);
  [DllImport("rstrtmgr.dll",CharSet=CharSet.Unicode)] static extern int RmRegisterResources(uint h,uint n,string[] f,uint na,IntPtr a,uint ns,string[] s);
  [DllImport("rstrtmgr.dll")] static extern int RmGetList(uint h,out uint n,ref uint c,[In,Out] INFO[] info,ref uint reason);
  public static List<string> Who(string path){
    var res=new List<string>(); uint h; var key=Guid.NewGuid().ToString();
    if(RmStartSession(out h,0,key)!=0) return res;
    try{
      if(RmRegisterResources(h,1,new[]{path},0,IntPtr.Zero,0,null)!=0) return res;
      uint n=0,c=0,reason=0;
      int rc=RmGetList(h,out n,ref c,null,ref reason);
      if(rc==234){ var arr=new INFO[n]; c=n;
        if(RmGetList(h,out n,ref c,arr,ref reason)==0)
          for(int i=0;i<n;i++) res.Add(arr[i].Process.dwProcessId+"\t"+arr[i].strAppName+"\t"+arr[i].strServiceShortName);
      }
    } finally { RmEndSession(h); }
    return res;
  }
}
"@
Add-Type -TypeDefinition $src -Language CSharp

if (-not (Test-Path $Path)) { Write-Host "파일이 없다 : $Path"; return }
$full = (Resolve-Path $Path).Path
$r = [RM]::Who($full)
if (-not $r) { Write-Host "잠근 프로세스 없음 (재시작 관리자 기준) : $full"; return }
Write-Host "[$full] 를 쥐고 있는 프로세스"
foreach ($x in $r) {
    $a = $x -split "`t"
    Write-Host ("  PID {0,-8} {1}{2}" -f $a[0], $a[1], $(if($a[2]){" (서비스 " + $a[2] + ")"}))
}
