using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.InteropServices;
using System.Security;

using System.Drawing;
using System.Drawing.Imaging;

namespace ECP.Gui.Util
{
    public class FillColorFiter
    {
        private byte fillA = 0;
        private byte fillR = 0;
        private byte fillG = 0;
        private byte fillB = 0;

        public ARGB FillColor
        {
            get { return new ARGB(fillA,fillR, fillG, fillB); }
            set
            {
                fillA = value.Alpha;
                fillR = value.Red;
                fillG = value.Green;
                fillB = value.Blue;
            }
        }

        public Bitmap Apply(Bitmap image)
        {
            // lock source bitmap data
            BitmapData srcData = image.LockBits(
                new Rectangle(0, 0, image.Width, image.Height),
                ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);

            // apply the filter
            Bitmap dstImage = Apply(srcData);

            // unlock source image
            image.UnlockBits(srcData);

            return dstImage;
        }

        public  Bitmap Apply(BitmapData imageData)
        {
            if (imageData.PixelFormat != PixelFormat.Format32bppArgb)
                throw new ArgumentException();

            // get image dimension
            int width = imageData.Width;
            int height = imageData.Height;

            // create new image
            Bitmap dstImage = new Bitmap(width, height, PixelFormat.Format32bppArgb);

            // lock destination bitmap data
            BitmapData dstData = dstImage.LockBits(
                new Rectangle(0, 0, width, height),
                ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);

            // copy image
            memcpy(dstData.Scan0, imageData.Scan0, imageData.Stride * height);

            // process the filter
            ProcessFilter(dstData);

            // unlock destination images
            dstImage.UnlockBits(dstData);

            return dstImage;
        }

        protected unsafe void ProcessFilter(BitmapData imageData)
        {
            int width = imageData.Width;
            int height = imageData.Height;
            int offset = imageData.Stride - width * 4;

            double originRatio = 0.5;
     
            // do the job
            byte* ptr = (byte*)imageData.Scan0.ToPointer();
            byte a,r, g, b;
      
            // for each row
            for (int y = 0; y < height; y++)
            {
                // for each pixel
                for (int x = 0; x < width; x++, ptr += 4)
                {
                    a = ptr[ARGB.A];
                    r = (byte)(ptr[ARGB.R] * (originRatio ));
                    g = (byte)(ptr[ARGB.G] * (originRatio ));
                    b = (byte)(ptr[ARGB.B] * (originRatio ));
            
                    if(a != 0 && 
                        !(fillR==0 &&
                          fillG==0 &&
                          fillB==0))
                    {
                        if ((int)a + (int)fillA >= 255)
                        {
                            ptr[ARGB.A] = 255;
                        }
                        else
                        {
                            ptr[ARGB.A] = (byte)(a + fillA);
                        }

                        if ((int)r + (int)fillR >= 255)
                        {
                            ptr[ARGB.R] = 255;
                        }
                        else
                        {
                            ptr[ARGB.R] = (byte)(r + fillR);
                        }

                        if ((int)g + (int)fillG >= 255)
                        {
                            ptr[ARGB.G] = 255;
                        }
                        else
                        {
                            ptr[ARGB.G] = (byte)(g + fillG);
                        }
                        if ((int)b + (int)fillB >= 255)
                        {
                            ptr[ARGB.B] = 255;
                        }
                        else
                        {
                            ptr[ARGB.B] = (byte)(b + fillB);
                        }
                    }
                }
                ptr += offset;
            }
        }

        [DllImport("ntdll.dll")]
        public static extern IntPtr memcpy(
            IntPtr dst,
            IntPtr src,
            int count);

    }
    public class ARGB
    {

        /// <summary>
        /// Index of a component
        /// </summary>
        public const short A = 3;
        /// <summary>
        /// Index of red component
        /// </summary>
        public const short R = 2;
        /// <summary>
        /// Index of green component
        /// </summary>
        public const short G = 1;
        /// <summary>
        /// Index of blue component
        /// </summary>
        public const short B = 0;

        /// <summary>
        /// Alpha component
        /// </summary>
        public byte Alpha;

        /// <summary>
        /// Red component
        /// </summary>
        public byte Red;

        /// <summary>
        /// Green component
        /// </summary>
        public byte Green;

        /// <summary>
        /// Blue component
        /// </summary>
        public byte Blue;

        /// <summary>
        /// <see cref="System.Drawing.Color">Color</see> value of the class 
        /// </summary>
        public System.Drawing.Color Color
        {
            get { return Color.FromArgb(Alpha,Red, Green, Blue); }
            set
            {
                Alpha = value.A;
                Red = value.R;
                Green = value.G;
                Blue = value.B;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RGB"/> class
        /// </summary>
        public ARGB() { }

        /// <summary>
        /// Initializes a new instance of the <see cref="RGB"/> class
        /// </summary>
        /// 
        /// <param name="red">Red component</param>
        /// <param name="green">Green component</param>
        /// <param name="blue">Blue component</param>
        /// 
        public ARGB(byte alpah, byte red, byte green, byte blue)
        {
            this.Alpha = alpah;
            this.Red = red;
            this.Green = green;
            this.Blue = blue;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RGB"/> class
        /// </summary>
        /// 
        /// <param name="color">Initialize from specified <see cref="System.Drawing.Color">color</see></param>
        /// 
        public ARGB(System.Drawing.Color color)
        {
            this.Alpha = color.A;
            this.Red = color.R;
            this.Green = color.G;
            this.Blue = color.B;
        }
    }
  
}
