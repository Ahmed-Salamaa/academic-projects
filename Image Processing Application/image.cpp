#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>

// Note : You must include the 'stb_image.h' and 'stb_image_write.h' libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// Primary image data
int width , height , channels ;
vector < vector < vector <int> > > image ;
unsigned char* data_pointer ;

// Secondary image data
int width2 , height2 , channels2 ;
vector < vector < vector <int> > > image2 ;
unsigned char* data_pointer2 ;

const int dx[] = { 0 , 1 , 1 , 1 } ;
const int dy[] = { 1 , -1 , 0 , 1 } ;
const int dz[] = { 7 , 3 , 5 , 1 } ;

void input_matrix_file() ;
void output_matrix_file() ;

bool print = 1 ;

void input_image ( vector < vector < vector <int> > > & tt = image , int& w = width , int& h = height , int& c = channels )
{
    string answer ;
    
    cout << "\n\nEnter the image file (e.g., image.png, supports PNG, JPEG)\n";
    cout << "Source: " ;
    cin >> answer ;
    data_pointer = stbi_load ( answer.c_str() , &w , &h , &c , 0 ) ;

    while ( !data_pointer ) 
    {
        cout << "Invaild name , try again : " ;
        cin >> answer ;
        data_pointer = stbi_load ( answer.c_str() , &w , &h , &c , 0 ) ;
    }

    tt.resize( h , vector < vector <int> > ( w , vector <int> ( c ) ) ) ;

    for ( int row = 0 ; row < h ; row ++ ) 
    {
        for ( int col = 0 ; col < w ; col ++ ) 
        {
            int index = ( row * w + col ) * c ;
            for ( int cc = 0 ; cc < c ; cc ++ ) 
                tt[row][col][cc] = data_pointer[index + cc] ;
        }
    }
    stbi_image_free(data_pointer);
    stbi_image_free(data_pointer2);

    cout << "Image Imported\n" ;
}

void output_image ( vector < vector < vector <int> > > & tt = image , int& w = width , int& h = height , int& c = channels )
{
    cout << "\n\nwrite image name with with extension (e.g., image.jpg) : " ;
    string ss ;
    cin >> ss ;

    vector <unsigned char> output_image ;
    for (int row = 0; row < h; ++row)
        for (int col = 0; col < w; ++col)
            for (int cc = 0; cc < c; ++cc)
                output_image.push_back(static_cast<unsigned char>(tt[row][col][cc]));

    stbi_write_png( ss.c_str() , w , h , c , output_image.data() , w * c ) ;
    cout << "Image saved as " << ss << "\n" ;
}

void input_matrix(  )
{
    cout << "Please enter the number of rows : " ;
    cin >> height ;
    cout << "Please enter the number of columns : ";
    cin >> width ;
    channels = 1 ;

    if (height * width > 1000)
    {
        cout << "Error: Matrix size too large.\n";
        cout << "Matrix will import from the file\n" ;
        input_matrix_file() ;
        return;
    }

    image.resize( height , vector < vector <int> > ( width , vector <int> ( channels ) ) ) ;

    for (int row = 0; row < height; ++row)
        for (int col = 0; col < width; ++col)
            cin >> image[row][col][0] ;

    cout << "Matrix Imported\n" ;
}

void output_matrix()
{
    if (height * width > 1000)
    {
        cout << "Error: Matrix size too large.\n";
        cout << "Matrix will export from the file\n" ;
        output_matrix_file() ;
        return;
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++) cout << image[row][col][0] << " " ;
        cout << "\n" ;
    }

    cout << "Matrix Exported\n" ;
}

void input_matrix_file (  )
{
    string fname ;
    cout << "Enter matrix file name to import (e.g., matrix.txt) : " ;
    cin >> fname ;
    ifstream fin ( fname ) ;
    if ( !fin )
    {
        cout << "Cannot open file , aborting.\n" ;
        return ;
    }
    fin >> height >> width ;
    channels = 1 ;
    image.resize ( height , vector < vector <int> > ( width , vector <int> ( 1 ) ) ) ;
    for ( int i = 0 ; i < height ; i ++ )
        for ( int j = 0 ; j < width ; j ++ )
            fin >> image[i][j][0] ;
    fin.close () ;
    cout << "Matrix imported from file \"" << fname << "\"\n" ;
}

void output_matrix_file ()
{
    cout << "Enter file name to save matrix (e.g., matrix_out.txt) : " ;
    string fname ;
    cin >> fname ;
    ofstream fout ( fname ) ;
    if ( !fout )
    {
        cout << "Cannot open file for writing , aborting.\n" ;
        return ;
    }
    fout << height << " " << width << "\n" ;
    for ( int i = 0 ; i < height ; i ++ )
    {
        for ( int j = 0 ; j < width ; j ++ )
            fout << image[i][j][0] << " " ;
        fout << "\n" ;
    }
    fout.close () ;
    cout << "Matrix saved to file \"" << fname << "\"\n" ;
}

void binary( int x , int y , int target )
{
    image[x][y][0] = ( image[x][y][0] > target ) ? 255 : 0 ;
}

void gray_image( vector < vector < vector <int> > > &tt = image , int &w = width , int &h = height , int &c = channels )
{
    if ( c == 1 ) return ;
    c = 1 ;
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            tt[row][col][0] = 0.299 * tt[row][col][0] + 0.587 * tt[row][col][1] + 0.114 * tt[row][col][2];
            tt[row][col].resize(1) ;
        }
    }
}

void dithering ( int target )
{
    gray_image() ;
    for ( int row = 0 ; row < height ; row ++ ) 
        for ( int col = 0 ; col < width ; col ++ ) 
            binary( row , col , target ) ;
}

void cell_Floyd ( int x , int y , int i , int e )
{
    if ( x < 0 || x >= height || y < 0 || y >= width ) return ;
    image[x][y][0] += dz[i] * e / 16.0 ;
}

void Floyd ()
{
    gray_image();
    for ( int row = 0 ; row < height ; row ++ ) 
    {
        for ( int col = 0 ; col < width ; col ++ ) 
        {
            int error = image[row][col][0] ;
            binary( row , col , 128 ) ;
            error -= image[row][col][0] ;

            for ( int i = 0 ; i < 4 ; i ++ ) cell_Floyd ( row + dx[i] , col + dy[i] , i , error ) ;
        }
    }
}

    int mask_size = 4 ;

vector < vector <int> > input_mask ()
{
    vector < vector <int> > mask = {
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };
    cout << "Use Bayer mask or input other Mask\n" ;
    cout << "1 . for Bayer mask\n" ;
    cout << "2 . for Input a mask\n" ;
    cout << "Your choice : " ;

    string type ;
    while (true)
    {
        cin >> type ;
        if ( type == "1" ) return mask ;
        else if ( type == "2" ) break;
        else cout << "Invaild input , try agin : " ;
    }
    
    cout << "Enter mask size (e.g., 4 for 4x4 matrix) : " ;
    cin >> mask_size ;
    mask.resize ( mask_size , vector <int> ( mask_size ) ) ;

    cout << "Enter the mask values row by row :\n" ;
    for ( int i = 0 ; i < mask_size ; i ++ )
        for ( int j = 0 ; j < mask_size ; j ++ )
            cin >> mask[i][j] ;

    
    cout << "Mask Imported\n" ;
    return mask ;
}

void ordered_dithering()
{
    vector < vector <int> > mask = input_mask() ;
    gray_image() ;

    int max_mask_value = 0;
    for (int i = 0; i < mask_size; i++) {
        for (int j = 0; j < mask_size; j++) {
            max_mask_value = max(max_mask_value, mask[i][j]);
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int threshold = (mask[row % mask_size][col % mask_size] * 255) / (max_mask_value + 1) ;
            image[row][col][0] = (image[row][col][0] > threshold) ? 255 : 0 ;
        }
    }

    cout << "Image converted using Ordered Dithering\n" ;
}

void pattern_dithering()
{
    vector < vector <int> > mask = input_mask() ;
    gray_image() ;

    int max_mask_value = 0 ;
    for (int i = 0; i < mask_size; i++)
        for (int j = 0; j < mask_size; j++)
            max_mask_value = max ( max_mask_value , mask[i][j] ) ;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int pixel_value = image[row][col][0] * max_mask_value / 255;
            int threshold = mask[row % mask_size][col % mask_size];
            image[row][col][0] = (pixel_value > threshold) ? 255 : 0;
        }
    }

    cout << "Image converted using Pattern Dithering\n";
}


void cross_dissolve()
{
    cout << "Loading second image for transition.\n";
    input_image(image2, width2, height2, channels2);

    if (width != width2 || height != height2 || channels != channels2) 
    {
        cout << "Error: Images must have identical dimensions and channels.\n";
        print = 0 ;
        return;
    }

    float alpha;
    cout << "Enter alpha (0.0 to 1.0): ";
    cin >> alpha;

    while ((alpha < 0.0 || alpha > 1.0))
    {
        cout << "Alpha must be between 0.0 and 1.0.\n";
        cout << "Enter alpha (0.0 to 1.0): ";
        cin >> alpha;
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            for (int c = 0; c < channels; c++)
            {
                float A = image[row][col][c];
                float B = image2[row][col][c];
                image[row][col][c] = (int)((1 - alpha) * A + alpha * B + 0.5);
            }
        }
    }
}

void dither_dissolve()
{
    cout << "Loading second image for transition.\n";
    input_image(image2, width2, height2, channels2);

    if (width != width2 || height != height2 || channels != channels2) 
    {
        cout << "Error: Images must have identical dimensions and channels.\n";
        print = 0 ;
        return;
    }

    float alpha;
    cout << "Enter alpha (0.0 to 1.0): ";
    cin >> alpha;

    while ((alpha < 0.0 || alpha > 1.0))
    {
        cout << "Alpha must be between 0.0 and 1.0.\n";
        cout << "Enter alpha (0.0 to 1.0): ";
        cin >> alpha;
    }
    

    int switch_col = (int)(alpha * width);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (col < switch_col)
            {
                for (int c = 0; c < channels; c++)
                {
                    image[row][col][c] = image2[row][col][c];
                }
            }
        }
    }
}


void info()
{
    system("CLS");

    const string APP_NAME = "Image Processing Application";
    const string VERSION = "2.01";
    const string DEVELOPER = "Ahmed Khaled";

    cout << "\n" << string(50, '=') << "\n";
    cout << "          " << APP_NAME << "          \n";
    cout << string(50, '=') << "\n";

    cout << "Version:       " << VERSION << "\n";
    cout << "Developed by:  " << DEVELOPER << "\n";
    cout << string(50, '-') << "\n";

    cout << "Features:\n";
    cout << "  - Import/export images (PNG, JPEG, etc.) via stb_image.\n";
    cout << "  - Input/output matrices via console or text files.\n";
    cout << "  - Convert images to grayscale.\n";
    cout << "  - Apply dithering: Native, Brightness-based, Floyd-Steinberg.\n";
    cout << "  - Support for Ordered and Pattern dithering with custom masks.\n";
    cout << "  - Perform image transitions: Cross Dissolve, Dither Dissolve.\n";
    cout << string(50, '-') << "\n";

    cout << "Current Image Status:\n";
    if (width > 0 && height > 0 && channels > 0)
    {
        cout << "  - Dimensions: " << width << " x " << height << " pixels\n";
        cout << "  - Channels:   " << channels << (channels == 1 ? " (Grayscale)" : " (RGB)") << "\n";
    }
    else cout << "  - No image loaded.\n" ;
    
    cout << string(50, '-') << "\n";

    cout << "Thank you for using the " << APP_NAME << "!\n";
}

void pause()
{
    cout << "\nPress Enter to continue..." ;
    cin.ignore() ;
    // cin.get() ;
}

void choose_input_source ( vector < vector < vector <int> > > & tt = image , int& w = width , int& h = height , int& c = channels )
{
    cout << "\n\n\n" ;
    cout << "Choose input source:\n" ;
    cout << " 1 : Image\n" ;
    cout << " 2 : Matrix (console)\n" ;
    cout << " 3 : Import Matrix from File\n" ;
    cout << "Your choice : " ;

    string ch ;
    cin >> ch ;
    cin.ignore() ;

    if ( ch == "1" ) input_image ( tt , w , h , c ) ;
    else if ( ch == "2" ) input_matrix () ;
    else if ( ch == "3" ) input_matrix_file () ;
    else { cout << "Invalid choice. Defaulting to Image.\n" ; input_image () ; }
}

void choose_output_destination ( vector < vector < vector <int> > > & tt = image , int& w = width , int& h = height , int& c = channels )
{
    cout << "\n\n\n" ;
    cout << "Choose output destination:\n" ;
    cout << " 1 : Image file\n" ;
    cout << " 2 : Print Matrix to console\n" ;
    cout << " 3 : Export Matrix to File\n" ;
    cout << "Your choice : " ;

    string ch ;
    cin >> ch ;
    cin.ignore() ;

    if ( ch == "1" ) output_image ( tt , w , h , c ) ;
    else if ( ch == "2" ) output_matrix () ;
    else if ( ch == "3" ) output_matrix_file () ;
    else { cout << "Invalid choice. Defaulting to Image file.\n" ; output_image () ; }
}

int main() 
{
    cout << "\n           Welcome to the Image Processing App\n" ;
    while (true)
    {
        int i = 1 ;
        vector <string> menuOptions =
        {
            "Convert Image to Grayscale" ,
            "Apply Native Threshold Dithering" ,
            "Apply Brightness-based Threshold Dithering" ,
            "Apply Floyd-Steinberg Error Diffusion" ,
            "Apply Ordered Dithering" ,
            "Apply Pattern Dithering" ,
            "Apply Cross Dissolve Transition" ,
            "Apply Dither Dissolve Transition" ,
            "INFO" ,
            "EXIT"
        } ;
        
        cout << "\n================== MAIN MENU ==================\n" ;
        for ( const auto& option : menuOptions ) cout << " " << i ++ << " : " << option << "\n" ;
        cout << "===============================================\n" ;
        cout << "Please enter your choice: " ;

        string type ;
        cin >> type ;
        cin.ignore() ;

        i = 1 ;

        int tt = stoi(type) ;
        print = 1 ;

        if ( 1 <= tt && tt <= (int)( menuOptions.size() - 2 ) ) choose_input_source () ;

             if ( type == to_string( i ++ ) ) gray_image() , cout << "Image converted to Grayscale form \n" ;
        else if ( type == to_string( i ++ ) ) dithering(128) , cout << "Image converted to Native Threshold Dithering form \n" ;
        else if ( type == to_string( i ++ ) )
        {
            long long answer = 0 ;
            gray_image() ;
            for ( int row = 0 ; row < height ; row ++ ) 
                for ( int col = 0 ; col < width ; col ++ ) 
                    answer += image[row][col][0] ;

            dithering(answer/(height*width)) ;
            cout << "Image converted to Brightness-based Threshold Dithering form \n" ;
        }
        else if ( type == to_string( i ++ ) ) Floyd ( ) , cout << "Image converted to Floyd-Steinberg Error Diffusion form \n" ;
        else if ( type == to_string( i ++ ) ) ordered_dithering () , cout << "Image converted to Ordered Dithering form \n" ;
        else if ( type == to_string( i ++ ) ) pattern_dithering () , cout << "Image converted to Pattern Dithering form \n" ;
        else if ( type == to_string( i ++ ) ) cross_dissolve() , cout << "Cross Dissolve applied\n" ;
        else if ( type == to_string( i ++ ) ) dither_dissolve() , cout << "Dither Dissolve applied\n" ;
        else if ( type == to_string( i ++ ) ) info() ;
        else if ( type == to_string( i ++ ) ) { cout << "Thanks for using our app!" ; return 0 ; }
        else cout << "Invalid input. Please try again." ;

        if ( 1 <= tt && tt <= (int)( menuOptions.size() - 2 ) && print ) choose_output_destination () ;

        pause() ;
        system("CLS") ;
    }
}