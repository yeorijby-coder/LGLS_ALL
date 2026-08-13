using System;
using System.Collections.Generic;
using System.Text;

using System.Threading;
using ECP.Device.Observe;

namespace ECP.Device.Observe
{
    public class ObservableUtil
    {

        public static bool TimeOut(Observable observable, object targetValue, int scanInterval, int timeoutSecond)
        {
            DateTime now = DateTime.Now;

            bool isTimeOut = false;

            do
            {
                if (((TimeSpan)(DateTime.Now - now)).TotalSeconds > timeoutSecond)
                {
                    isTimeOut = true;
                    break;
                }
                //if (!observable.IsMonitoring)
                //{
                    observable.ReadValueSync(false);
                //}

                Thread.Sleep(scanInterval);

            } while (observable.Value.Equals(targetValue));

            return isTimeOut;
        }


        public static bool TimeOut( object targetValue, int scanInterval, int timeoutSecond, params Observable[] observableList)
        {
            DateTime now = DateTime.Now;
            bool isTimeOut = false;
            bool done = false;
            int matchCount = 0;

            do
            {
                matchCount = 0;
                if (((TimeSpan)(DateTime.Now - now)).TotalSeconds > timeoutSecond)
                {
                    isTimeOut = true;
                    break;
                }
                for (int i = 0; i < observableList.Length; i++)
                {
                    if (!observableList[i].IsMonitoring)
                    {
                        observableList[i].ReadValueSync(false);
                    }
                    if (observableList[i].Value.Equals(targetValue)) matchCount++;
                    if (matchCount == observableList.Length) done = true;
                }
                Thread.Sleep(scanInterval);
            } while (!done);

            return isTimeOut;
        }

        public static void On(Observable observable, object targetValue, int delayTime, int interval)
        {
            DateTime now = DateTime.Now;
            do
            {
                if(((TimeSpan)(DateTime.Now - now)).TotalSeconds >= delayTime)
                {
                    observable.Value = targetValue; break;
                }
                Thread.Sleep(interval);

            } while(true);
        }

        public static void TriggerISecs(Observable observable, int interval)
        {
            observable.Value = true;

            DateTime now = DateTime.Now;
            do
            {
                if (((TimeSpan)(DateTime.Now - now)).TotalSeconds >= 1000)
                {
                    observable.Value = false; break;
                }
                Thread.Sleep(interval);
            } while (true);

        }

        public static void TriggerMultiSecs(Observable observable, int interval, int duration)
        {
            DateTime now = DateTime.Now;
            observable.Value = true;
            do
            {
                if (((TimeSpan)(DateTime.Now - now)).TotalSeconds >= duration)
                {
                    observable.Value = false; break;
                }
                Thread.Sleep(interval);
            } while (true);
        }


        public static void Increase(Observable observable, object targetValue, int step, int interval)
        {
            do
            {
                if (step > 0)
                {
                    if (observable.AsInteger >= (int)targetValue) break;
                }
                else
                {
                    if (observable.AsInteger <= (int)targetValue) break;
                }
                observable.AsInteger += step;
                Thread.Sleep(interval);


            } while (true);
        }

        public static void Increase(Observable observable, object targetValue, double step, int interval)
        {
            do
            {
                if (step > 0)
                {
                    if (observable.AsDouble >= (int)targetValue) break;
                }
                else
                {
                    if (observable.AsDouble <= (int)targetValue) break;
                }
                observable.AsDouble += step;
                Thread.Sleep(interval);


            } while (true);
        }

        public static string MakeObservableFilteringStatement(Dictionary<string, string> filteringCondition)
        {
            string sqlStatement = null;
            bool isFirstCondition = true;
            if (filteringCondition.Count > 0)
            {
                foreach (string key in filteringCondition.Keys)
                {
                    if (isFirstCondition)
                    {
                        sqlStatement = "where " + key + " like '%" + filteringCondition[key] + "%'\n";
                        isFirstCondition = false;
                    }
                    else
                    {
                        sqlStatement = sqlStatement + "and " + key + " like '%" + filteringCondition[key] + "%'\n";
                    }
                }
            }

            return sqlStatement;
        }
    }
}
