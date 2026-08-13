using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Configuration;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class GemDate
    {
        public const String DATE_FORMAT_YYMMDDHHMMSS = "yyMMddHHmmss";
        public const String DATE_FORMAT_YYYYMMDDHHMMSSCC = "yyyyMMddHHmmssffff";

        public static String GetDate(bool centuryFormat)
        {
            return GetDate(centuryFormat ? DATE_FORMAT_YYYYMMDDHHMMSSCC :
                    DATE_FORMAT_YYMMDDHHMMSS);
        }


        public static String GetDate(String format)
        {
            string date = DateTime.Now.ToString(format, DateTimeFormatInfo.InvariantInfo);

            // This format I need to cut down to centi-seconds.
            if (format.Equals(DATE_FORMAT_YYYYMMDDHHMMSSCC))
            {
                date = date.Substring(0, 16);
            }
            return date;
        }


        public static DateTime ParseDate(String secsDate)
        {
            string format;

            if (secsDate.Length == DATE_FORMAT_YYMMDDHHMMSS.Length)
            {
                format = DATE_FORMAT_YYMMDDHHMMSS;
            }
            else
            {
                secsDate += "00";
                format = DATE_FORMAT_YYYYMMDDHHMMSSCC;
            }

            try
            {
                if (format.Equals(DATE_FORMAT_YYMMDDHHMMSS))
                {
                    int year = Convert.ToInt32(secsDate.Substring(0, 2)) + 2000; // we assume, 2000 year
                    int month = Convert.ToInt32(secsDate.Substring(2, 2));
                    int day = Convert.ToInt32(secsDate.Substring(4, 2));
                    int hour = Convert.ToInt32(secsDate.Substring(6, 2));
                    int minitues = Convert.ToInt32(secsDate.Substring(8, 2));
                    int second = Convert.ToInt32(secsDate.Substring(10, 2));

                    return new DateTime(year, month, day, hour, minitues, second);
                }
                else if (format.Equals(DATE_FORMAT_YYYYMMDDHHMMSSCC))
                {
                    int year = Convert.ToInt32(secsDate.Substring(0, 4));
                    int month = Convert.ToInt32(secsDate.Substring(4, 2));
                    int day = Convert.ToInt32(secsDate.Substring(6, 2));
                    int hour = Convert.ToInt32(secsDate.Substring(8, 2));
                    int minitues = Convert.ToInt32(secsDate.Substring(10, 2));
                    int second = Convert.ToInt32(secsDate.Substring(12, 2));
                    int mili = Convert.ToInt32(secsDate.Substring(14, 2));

                    return new DateTime(year, month, day, hour, minitues, second, mili);
                }
            }
            catch (Exception ex)
            {
                //
            }
            return new DateTime();
        }
    }
}
