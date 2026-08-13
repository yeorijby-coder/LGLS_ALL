using System;
using System.Collections.Generic;
using System.Text;

using ECP.Global.Exception;
using ECP.Global;
using ECP.Db;

using ECP.Util.UDProperty;
using ECP.Global.Service;
using ECP.Service.Logger;

namespace ECP.Device
{
    /// <summary>
    /// User Defined Property 기능을 담고 있는 Class
    /// User Defined Property Capability를 가지기를 원하는 Element는
    /// 이 Class을 상속해서 사용해야 한다. 
    /// 
    /// Element는 일반적으로 Persistence기능을 사용하여,
    /// 자신의 Property들을 Data base에 관리 할 수 있으나,
    /// 
    /// 어떤 경우에 한해서는 특정 장비 또는 특정 Project에서만
    /// Specific하게 관리되는 Property를 사용하게 되는 경우가 있다.
    /// 
    /// 이경우는 Product의 Data base Table에 Column을 늘이는 대신
    /// User Defined Property 기능을 사용하게 되면,
    /// 여러 Type의 Element들에 모두 사용 할 수 있는 DeviceProperty Table에
    /// Cloumn의 추가 수정 없이, 원하는 User Defined Property를 
    /// 추가 할 수 있다.
    /// 
    /// </summary>
    [Serializable]
    public abstract class AbstractUDPElement : Persistence
    {
        /// <summary>
        /// Class Name을 담고 있는 Property
        /// </summary>
        const string CLASS = "AbstractUDPElement";

        /// <summary>
        /// 이 Element의 User Define Property을 담고 있는 Container
        /// </summary>
        private Dictionary<string, UserDefinedProperty> userDefinedProperties = 
            new Dictionary<string, UserDefinedProperty>();

        /// <summary>
        /// 이 Element의 User Define Property을 담고 있는 Container
        /// </summary>
        private Dictionary<string, UserDefinedProperty> UDP
        {
            get { return userDefinedProperties; }
            set { userDefinedProperties = value; }
        }

        /// <summary>
        /// User Defined Property의 Thread Safe을 보장하기 위한 monitor 객체
        /// </summary>
        private static object udplist_monitor = new object();
     
        /// <summary>
        /// 이 Mehtod는 상속하는 Class에서 Override 구현되어져야 함
        /// 저장하고자하는 Userdefined Property의 EntityClass Name을 Return함
        /// </summary>
        /// <returns>저장하고자하는 Userdefined Property의 EntityClass Name을 Return</returns>
        abstract public string GetUserDefinedEntityClassName();

        /// <summary>
        /// 이 Mehtod는 상속하는 Class에서 Override 구현되어져야 함
        /// 저장하고자하는 Userdefined Property의 Entity Id을 Return함
        /// </summary>
        /// <returns>Entity Id</returns>
        abstract public string GetUserDefineEntityId();

        /// <summary>
        /// User Defined Property를 Element에 추가
        /// </summary>
        /// <param name="name">User Defined Property의 Name</param>
        /// <param name="value">User Defined Property의 Value</param>
        /// <param name="requestor">이것을 요구한 요청자</param>
        public void AddUserDefinedProperty(string name, string value, string requestor)
        {
            const string METHOD = CLASS + "." + "AddUserDefinedProperty";

            try
            {
                UserDefinedProperty udAttr = null;

                lock (udplist_monitor)
                {

                    if (userDefinedProperties.ContainsKey(name))
                    {
                        throw new ECPException(ECPExceptionType.DUPLICATE, name, "User Defined Property(" + name +
                                               ") already exists ");
                    }
                    else
                    {
                        udAttr = UserDefinedProperty.NewUserDefinedProperty(GetUserDefinedEntityClassName(),
                            GetUserDefineEntityId(), name, value);

                        this.userDefinedProperties.Add(name, udAttr);

                    }
                }

            }
            catch(ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "AddUserDefinedProperty", "debug", ECP.Service.Logger.Category.DEBUG, "", "AddUserDefinedProperty", ex.Message.ToString());
            }
        }

        /// <summary>
        /// 주어진 Name에 해당하는 User Defined Property의 값을 Return
        /// </summary>
        /// <param name="name">요구하고자 하는 User Defined Property의 Name</param>
        /// <returns>요구하는 Name에 대한 값</returns>
        public string GetUserDefinedPropertyValue(string name)
        {
            const string METHOD = CLASS + "." + "GetUserDefinedPropertyValue";

            try
            {

                UserDefinedProperty udAttr = null;

                if (this.userDefinedProperties.ContainsKey(name))
                {
                    return this.userDefinedProperties[name].Value;
                }
                else
                {
                    throw new ECPException(ECPExceptionType.NOT_FOUND, name, "User Defined Property (" + name +
                        ") not found");
                }
            }
            catch (ECPException ex)
            {
                Log.log(5, "GetUserDefinedPropertyValue", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetUserDefinedPropertyValue", ex.Message.ToString());
            }
            catch (Exception ex)
            {
                Log.log(5, "GetUserDefinedPropertyValue", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetUserDefinedPropertyValue", ex.Message.ToString());
            }
            return null;
        }

        /// <summary>
        /// User Define 값을 가져 옴. 그러나 DB에 Define 값이 없을 경우를 대비하여
        /// default value 및 속성을 DB에 생성 할 수 있음.
        /// </summary>
        /// <param name="name">user define 명</param>
        /// <param name="defaultValue">defalut value</param>
        /// <param name="forceCreateProperty"> true : user Define 생성, false : 생성하지 않음</param>
        /// <returns>value</returns>
        public string GetUserDefinedPropertyValue(string name, string defaultValue, bool forceCreateProperty)
        {
            const string METHOD = CLASS + "." + "GetUserDefinedPropertyValue";

            try
            {

                UserDefinedProperty udAttr = null;

                if (this.userDefinedProperties.ContainsKey(name))
                {
                    return this.userDefinedProperties[name].Value;
                }
                else
                {
                    if (forceCreateProperty)
                        this.AddUserDefinedProperty(name, defaultValue, "");

                    return defaultValue;
                }
            }
            catch (ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "GetUserDefinedPropertyValue", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetUserDefinedPropertyValue", ex.Message.ToString());
            }
            return null;
        }

        /// <summary>
        /// User Defined Property의 값을 변경, 만약 주어진 Name에 해당하는 User Defined Property가 없을 경우는
        /// 추가함
        /// </summary>
        /// <param name="name">추가 또는 변경하고자하는 Property의 Name</param>
        /// <param name="value">추가 또는 변경하고자 하는 Property의 값</param>
        /// <param name="requestor">요청자</param>
        public void SetUserDefinedPropertyValue(string name, string value, string requestor)
        {
            const string METHOD = CLASS + "." + "SetUserDefinedPropertyValue";

            try
            {
                lock (udplist_monitor)
                {
                    UserDefinedProperty udAttr = null;

                    if (this.userDefinedProperties.ContainsKey(name))
                    {
                        this.userDefinedProperties[name].Value = value;
                    }
                    else
                    {
                        this.AddUserDefinedProperty(name, value, requestor);
                        return;

                    }
                }
            }
            catch (ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "SetUserDefinedPropertyValue", "debug", ECP.Service.Logger.Category.DEBUG, "", "SetUserDefinedPropertyValue", ex.Message.ToString());
            }
        }

        /// <summary>
        /// User Defined Property를 삭제
        /// </summary>
        /// <param name="name">삭제하고자 하는 Property의 Name</param>
        /// <param name="requestor">요청자</param>
        public void DeleteUserDefinedProperty(string name, string requestor)
        {
            const string METHOD = CLASS + "." + "DeleteUserDefinedProperty";

            try
            {
                lock (udplist_monitor)
                {

                    if (this.userDefinedProperties.ContainsKey(name))
                    {

                       // this.userDefinedProperties.Remove(name);
                        this.userDefinedProperties[name].Delete();
                        this.userDefinedProperties.Remove(name);
                    }
                    else
                    {
                        throw new ECPException(ECPExceptionType.NOT_FOUND, name, "User Defined Property (" + name +
                            ") not found");
                    }
                }
            }
            catch (ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "DeleteUserDefinedProperty", "debug", ECP.Service.Logger.Category.DEBUG, "", "DeleteUserDefinedProperty", ex.Message.ToString());
            }
        }

        /// <summary>
        /// 이 Element가 가지고 있는 모든 User defined property를 Return함
        /// </summary>
        /// <returns>Element의 모든 Property값들</returns>
        public List<UserDefinedPropertyInfo> GetUserDefinedProperties()
        {
            const string METHOD = CLASS + "." + "GetUserDefinedProperties";

            List<UserDefinedPropertyInfo> userDefinedPropertyInfoes = new List<UserDefinedPropertyInfo>();

            try
            {
                lock (udplist_monitor)
                {

                    foreach (UserDefinedProperty udp in this.userDefinedProperties.Values)
                    {
                        userDefinedPropertyInfoes.Add(new UserDefinedPropertyInfo(udp.Name, udp.Value));
                    }                

                    return userDefinedPropertyInfoes;
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "GetUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetUserDefinedProperties", ex.Message.ToString());
            }
            return null;
        }

        /// <summary>
        /// 이 Element가 가진 모든 Property를 삭제함
        /// </summary>
        /// <param name="requestor">삭제 요청자</param>
        public void DeleteAllUserDefinedProperties(string requestor)
        {
            const string METHOD = CLASS + "." + "DeleteAllUserDefinedProperties";

            try
            {
                lock (udplist_monitor)
                {
                    UserDefinedProperty.DeleteAllForEntity(GetUserDefinedEntityClassName(), GetUserDefineEntityId());
                    this.userDefinedProperties.Clear();
                }
            }
            catch (ECPException ex)
            {
                Log.log(5, "DeleteAllUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "DeleteAllUserDefinedProperties", ex.Message.ToString());
            }
            catch (Exception ex)
            {
                Log.log(5, "DeleteAllUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "DeleteAllUserDefinedProperties", ex.Message.ToString());
            }
        }

        /// <summary>
        /// 이 Element가 가진 모든 Proeprty들을 Database로 부터 Load함
        /// </summary>
        public void LoadUserDefinedProperties()
        {
            try
            {
                this.userDefinedProperties = UserDefinedProperty.
                    GetUserDefinedPropertiesForEntity(GetUserDefinedEntityClassName(), GetUserDefineEntityId());
            }
            catch (ECPException ex)
            {
                Log.log(5, "LoadUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "LoadUserDefinedProperties", "Sql Error");
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "LoadUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "LoadUserDefinedProperties", "Load error");
                throw ex;
            }
        }
    }
}
