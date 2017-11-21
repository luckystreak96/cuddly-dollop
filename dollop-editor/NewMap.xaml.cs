using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for NewMap.xaml
    /// </summary>
    public partial class NewMap : Window
    {
        private int _mapWidth;
        public int MapWidth { get { return _mapWidth; } set { _mapWidth = value; txtWidth.Text = _mapWidth.ToString(); } }
        private int _mapHeight;
        public int MapHeight { get { return _mapHeight; } set { _mapHeight = value; txtHeight.Text = _mapHeight.ToString(); } }
        public NewMap()
        {
            InitializeComponent();
            MapWidth = -1;
            MapHeight = -1;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            int.TryParse(txtWidth.Text, out int x);
            int.TryParse(txtHeight.Text, out int y);
            if(x > 0 && y > 0)
            {
                MapWidth = x;
                MapHeight = y;
            }

            Close();
        }
    }
}
