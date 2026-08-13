using System;
using System.Collections.Generic;
using System.Text;

using ECP.Global;

namespace ECP.Global.Gui
{
    [Serializable]
    public class WidgetData : ICloneable
    {
        public Guid guId;

        public string widgetAssemblyName = null;

        public string widgetClassName = null;

        public string elementId = null;

        public int x = 0;

        public int y = 0;

        public int width = 0;

        public int height = 0;

   
        public UserDefinedPropertyInfo[] properties = null;

        public WidgetData()
        {
        }

        public WidgetData(Guid guId, string widgetClassName, string elementId,
            int x, int y, int width, int height)
        {
            this.guId = guId;
            this.widgetClassName = widgetClassName;

            this.elementId = elementId;
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
        }

        public WidgetData(Guid guId, string widgetAssemblyName, string widgetClassName, string elementId,
            int x, int y, int width, int height)
        {
            this.guId = guId;
            this.widgetAssemblyName = widgetAssemblyName;
            this.widgetClassName = widgetClassName;

            this.elementId = elementId;
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
        }

        public WidgetData(Guid guId, string widgetClassName, string elementId,
            int x, int y, int width, int height, UserDefinedPropertyInfo[] properties)
        {
            this.guId = guId;
            this.widgetClassName = widgetClassName;

            this.elementId = elementId;
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
            this.properties = properties;
        }

        public WidgetData(Guid guId, string widgetAssemblyName, string widgetClassName, string elementId,
            int x, int y, int width, int height, UserDefinedPropertyInfo[] properties)
        {
            this.guId = guId;
            this.widgetAssemblyName = widgetAssemblyName;
            this.widgetClassName = widgetClassName;

            this.elementId = elementId;
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
            this.properties = properties;
        }

   
        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}
