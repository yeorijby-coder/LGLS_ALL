using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace ECP.Interfaces.Emulator
{
    public enum enumMemoryTpe
    {
        Attribute,
        Element,
        File
    };

    public interface ISimulation
    {
        int GetValue(string sUniqueID, out object newValue);
        int SetValue(string sUniqueID, object newValue);

        ////int GetValue(string sUniqueID, out bool newValue);
        ////int SetValue(string sUniqueID, bool newValue);

        ////int GetValue(string sUniqueID, out byte newValue);
        ////int SetValue(string sUniqueID, byte newValue);

        ////int GetValue(string sUniqueID, out short newValue);
        ////int SetValue(string sUniqueID, short newValue);

        ////int GetValue(string sUniqueID, out int newValue);
        ////int SetValue(string sUniqueID, int newValue);

        ////int GetValue(string sUniqueID, out float newValue);
        ////int SetValue(string sUniqueID, float newValue);

        ////int GetValue(string sUniqueID, out double newValue);
        ////int SetValue(string sUniqueID, double newValue);

        ////int GetValue(string sUniqueID, out string newValue);
        ////int SetValue(string sUniqueID, string newValue);
    }
}
