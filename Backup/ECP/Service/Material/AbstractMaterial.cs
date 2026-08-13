using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;

namespace ECP.Service.Material
{
    public class AbstractMaterial : Persistence
    {
        private const string CLASS = "Material";

        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }

        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

    }
}
