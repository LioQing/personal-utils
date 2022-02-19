using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Numerics;

namespace lio
{
    class CustomFloat
    {
        public readonly int ExponentSize;
        public readonly int SignificandSize;
        public readonly long Bias;

        public int Size { get { return ExponentSize + SignificandSize + 1; } }
        public static int ExponentPos { get { return 1; } }
        public int SignificandPos { get { return ExponentPos + ExponentSize; } }

        public BitArray Data { get; set; }

        public CustomFloat(int exponentSize, int significandSize, long bias)
        {
            ExponentSize = exponentSize;
            SignificandSize = significandSize;
            Bias = bias;
            Data = new(Size);
        }

        public CustomFloat(int exponentSize, int significandSize) 
            : this(exponentSize, significandSize, (1 << (exponentSize - 1)) - 1)
        {
        }

        public void SetValue<T>(T value)
        {
            if ((dynamic)value == 0)
            {
                Data.SetAll(false);
                return;
            }

            // parse

            Data[0] = (dynamic)value < 0;

            var integral = (BigInteger)Math.Abs(Math.Truncate((dynamic)value));
            var fractional = (double)Math.Abs((dynamic)value) - (double)integral;

            ExtractValue(integral, fractional);
        }

        public void SetValue(string value)
        {
            Data[0] = value[0] == '-';

            var dotPos = value.IndexOf('.');
            if (dotPos == -1)
                dotPos = value.Length;

            var integral = BigInteger.Parse(value[Convert.ToInt32(Data[0])..dotPos]);
            var fractional = double.Parse(value[dotPos..]);

            if (integral == 0 && fractional == 0)
            {
                Data.SetAll(false);
                return;
            }

            ExtractValue(integral, fractional);
        }

        private void ExtractValue(BigInteger integral, double fractional)
        {
            // integral binary
            var integralBin = new List<bool>();

            while (integral > 0)
            {
                integralBin.Add(integral % 2 == 1);
                integral /= 2;
            }

            integralBin.Reverse();

            var exponent = integralBin.Count + Bias - 1;

            // fraction binary
            var fractionalBin = new List<bool>();

            while (integralBin.Count == 0 && fractional * 2 <= 1)
            {
                fractional *= 2;
                --exponent;
            }

            for (var precision = SignificandSize - integralBin.Count + 1; precision > 0; precision--)
            {
                fractional *= 2;
                fractionalBin.Add(Convert.ToBoolean(Math.Truncate(fractional)));
                fractional -= Math.Truncate(fractional);
            }

            // save to Data

            var sig = integralBin.Concat(fractionalBin).ToArray();
            for (int i = 0; i < SignificandSize; ++i)
            {
                Data[SignificandPos + i] = sig[i + 1];
            }

            var exp = Convert.ToString(exponent, 2).PadLeft(ExponentSize, '0');
            for (int i = 0; i < ExponentSize; ++i)
            {
                Data[ExponentPos + i] = exp[i] == '1';
            }
        }

        public string ToBitString(int group_by = 4)
        {
            var sb = new StringBuilder();

            for (int i = 0; i < Data.Count; i++)
            {
                if ((Data.Count - i) % group_by == 0 && i != 0)
                    sb.Append(' ');

                char c = Data[i] ? '1' : '0';
                sb.Append(c);
            }

            return sb.ToString();
        }
    }
}
