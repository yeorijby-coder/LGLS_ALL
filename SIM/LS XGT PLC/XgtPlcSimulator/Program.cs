namespace XgtPlcSimulator;

internal static class Program
{
    [STAThread]
    static void Main()
    {
        ApplicationConfiguration.Initialize();
        Application.Run(new FRM_PLC_SERVER());
    }
}
