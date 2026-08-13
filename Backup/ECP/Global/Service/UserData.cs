using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Service
{
    [Serializable]
    public class UserData
    {
        public string userId = null;
        public string password = null;
        public string userName = null;
        public string userDescription = null;

        public DateTime passwordCreateTime = DateTime.Now;
        public int passwordMaxAge = 0;


        public UserData()
        {
        }

        public UserData(string _userId, string _password, string _userName,
            string _userDescription, DateTime _passwordCreateTime, int _passwordMaxAge)
        {
            userId = _userId;
            password = _password;
            userName = _userName;
            userDescription = _userDescription;
            passwordCreateTime = _passwordCreateTime;
            passwordMaxAge = _passwordMaxAge;
        } // ctor
    }
}
