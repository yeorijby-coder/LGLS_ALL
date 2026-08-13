using System;
using System.Collections.Generic;
using System.Text;

using ECP.Global.Exception;
using ECP.Global;
using ECP.Service.Logger;

namespace ECP.Util.UDProperty
{
    public abstract class AbstractUserDefindManager 
    {
        const string CLASS = "AbstractUserDefindManager";

        private Dictionary<string, UserDefinedProperty> userDefinedProperties = 
            new Dictionary<string, UserDefinedProperty>();

        public Dictionary<string, UserDefinedProperty> UDP
        {
            get { return userDefinedProperties; }
            set { userDefinedProperties = value; }
        }

        private static object udplist_monitor = new object();
     
        abstract public string GetUserDefinedEntityClassName();

        abstract public string GetUserDefineEntityId();

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

        public string GetUserDefinedPropertyValue(String name)
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
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "GetUserDefinedPropertyValue", "debug", ECP.Service.Logger.Category.DEBUG, "", "GetUserDefinedPropertyValue", ex.Message.ToString());
            }
            return null;
        }

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

        public void DeleteUserDefinedProperty(string name, string requestor)
        {
            const string METHOD = CLASS + "." + "DeleteUserDefinedProperty";

            try
            {
                lock (udplist_monitor)
                {

                    if (this.userDefinedProperties.ContainsKey(name))
                    {
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
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, "DeleteAllUserDefinedProperties", "debug", ECP.Service.Logger.Category.DEBUG, "", "DeleteAllUserDefinedProperties", ex.Message.ToString());
            }
        }

        public void LoadUserDefinedProperties()
        {
            try
            {
                this.userDefinedProperties = UserDefinedProperty.
                    GetUserDefinedPropertiesForEntity(GetUserDefinedEntityClassName(), GetUserDefineEntityId());
            }
            catch (ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
    }
}
