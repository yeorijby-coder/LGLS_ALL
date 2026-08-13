using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Gui
{

    [Serializable]
    public class TabData
    {
        public string tabName = null;

        public int tabBackGroundColor = 0;

        public List<WidgetData> widgetDataList = null;

        public TabData(string tabName, int tabBackGroundColor, List<WidgetData> widgetDataList)
        {
            this.tabName = tabName;
            this.tabBackGroundColor = tabBackGroundColor;
            this.widgetDataList = widgetDataList;
        }

        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}
