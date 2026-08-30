using System;
using System.Collections.Generic;

namespace HOST_SIM.Logic
{
    /// <summary>
    /// SC(스태커크레인) 담당 Bank 범위 안에서 랙 셀(BBbbLL)을 순차 할당한다.
    /// 증가 규칙: Bank 우선 (예: 010101 → 020101), 담당 Bank 소진 시 Bay+1, Bay 소진 시 Level+1, 이후 처음부터 순환.
    /// SC #k 담당 Bank = 2k-1, 2k  (STKSEM:1→RACK:1,2 ... STKSEM:5→RACK:9,10)
    /// </summary>
    public class RackAllocator
    {
        private readonly int[] banks;
        private readonly int maxBay;
        private readonly int maxLevel;
        private int bankIdx;
        private int bay;
        private int level;

        public RackAllocator(int[] banks, int maxBay, int maxLevel)
        {
            if (banks == null || banks.Length == 0) throw new ArgumentException("banks");
            this.banks = banks;
            this.maxBay = maxBay;
            this.maxLevel = maxLevel;
            bankIdx = 0;
            bay = 1;
            level = 1;
        }

        /// <summary>담당 Bank 개수 (셀 건너뛰기 시도 한도용)</summary>
        public int BankCount { get { return banks.Length; } }

        /// <summary>현재 셀 반환 (6자리 BBbbLL)</summary>
        public string Current
        {
            get
            {
                return banks[bankIdx].ToString().PadLeft(2, '0')
                     + bay.ToString().PadLeft(2, '0')
                     + level.ToString().PadLeft(2, '0');
            }
        }

        /// <summary>현재 셀을 반환하고 다음 셀로 증가</summary>
        public string Next()
        {
            string cell = Current;
            Advance();
            return cell;
        }

        private void Advance()
        {
            bankIdx++;
            if (bankIdx < banks.Length) return;
            bankIdx = 0;
            bay++;
            if (bay <= maxBay) return;
            bay = 1;
            level++;
            if (level <= maxLevel) return;
            level = 1;
        }

        /// <summary>재시작 시 상태 복원용</summary>
        public string StateText
        {
            get { return bankIdx + "," + bay + "," + level; }
            set
            {
                var p = value.Split(',');
                if (p.Length != 3) return;
                int bi, by, lv;
                if (int.TryParse(p[0], out bi) && int.TryParse(p[1], out by) && int.TryParse(p[2], out lv))
                {
                    if (bi >= 0 && bi < banks.Length && by >= 1 && by <= maxBay && lv >= 1 && lv <= maxLevel)
                    {
                        bankIdx = bi; bay = by; level = lv;
                    }
                }
            }
        }

        public static int[] BanksForScList(IEnumerable<int> scNos)
        {
            var list = new List<int>();
            foreach (int sc in scNos)
            {
                list.Add(sc * 2 - 1);
                list.Add(sc * 2);
            }
            list.Sort();
            return list.ToArray();
        }
    }
}
