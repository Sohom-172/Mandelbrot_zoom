#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// Setting the dimensions of the displaying window
const int W = 1920;
const int H = 1080;

// Setting the maximum number of itterations to be a fixed number
// This can be changed when we change the resolution 
int max_iter = 128;

// Initialising the zoom value to be 1
// This zoom can be changed when we uses the button features
double zoom = 1.0;

// Initialization of the maximum real and the minimum real value
double min_re = -2.5, max_re = 1;

// Initialising of the maximum imaginary and the minimum imaginary value
double min_im = -1, max_im = 1;

// computes v + t(u - v)
// t should be a value between 0 and 1

// Linear interpolation is required to color the RenderWindow accordingly
// Color return type is a pre defined return type for mixing 2 colors
// u and v mentioned below is the color code that are in the static vector assigned later in the program

Color linear_interpolation(const Color& v, const Color& u, double a)
{
	
	auto const b = 1 - a;
	return Color(b*v.r + a * u.r, b*v.g + a * u.g, b*v.b + a * u.b);

}


// Main function to write down the functionalities that are implemented
// Two types of functionalities are implemented

/*	
	Mouse :
--->Zoom in using the left key of the mouse button
--->Zoom out using the right key of the mouse button
--->Resolution increase using the upward scrolling of the mouse 
--->Resolution decrease using the downward scrolling of the mouse
*/

/*
	Keyboard:
---->Right key to scroll to the right side of the screen
---->Left key to scroll to the left side of the screen
---->Down key to scroll to the down side of the screen
---->Up key to scroll to the up side of the screen
*/

int main(void)
{
	
	//RenderWindow renders the window on which the Mandelbrot Set is being plotted
	RenderWindow window(VideoMode(W, H), "Mandelbrot Zoom");

	// Intialisation of the image object
	Image image; 
	
	// Creates the image on the rendered window
	image.create(W, H);
	
	// Adds texture to the Mandelbrot Set on the rendered window
	// Only role of the texture is to be loaded and mapped to graphical entities
	Texture texture;

	// Sprite is the class that encapsulates textures in SFML 
	Sprite sprite;
	
	// Initialization of the font object
	Font font; 

	// Loading the fonts from the downloaded arial.ttf file
	font.loadFromFile("arial.ttf");

	// Intialisation of the text object
	Text text;

	// Setting the font
	text.setFont(font);

	// Setting the character size 
	text.setCharacterSize(24);

	// Setting the filling color of the font 
	text.setFillColor(Color::White);

	// Checks if the window is open and executes the block
	while (window.isOpen())
	{
		//Intialisation of the event
		Event e;
		
		// Loop to check the status of the keyboard event
		while (window.pollEvent(e))
		{
			
			//Checks if the event is closed or not
			if (e.type == Event::Closed) 
			window.close();
			
			// Closes the window --> Termination of the event

			if (e.type == Event::KeyPressed)
			{
				//move delta

				double w = (max_re - min_re)*0.3;
				double h = (max_im - min_im)*0.3;

				//KEY PRESSES conditions

				// Traversing left side
				
				if (e.key.code == Keyboard::Left)
				// Checks if the key presses if left key or not 
				
				{ 
					// Changes the min_re value for traversing left
					// Substracts w value from the min_re value
					min_re -= w;

					// Changes the max_re value for traversing left
					// Substracts w value from the max_re value
					max_re -= w;
				}
				
				//Traversing right side

				if (e.key.code == Keyboard::Right)
				// Checks if the key presses if right key or not
				
				{ 
					
					// Changes the min_re value for traversing right
					// Adds w value from the min_re value
					min_re += w;

					// Changes the max_re value for traversing right
					// Adds w value from the max_re value
					max_re += w; 
				}
				
				//Traversing Upwards

				if (e.key.code == Keyboard::Up) 
				// Checks if the key press is up key or not

				{ 
					// Changes the min_im value for traversing upwards
					// Substracts h value from the min_im value
					min_im -= h;

					// Changes the max_im value for traversing upwards
					// Substracts h value from the max_im value
					max_im -= h; 
				}
		
				//Traversing Downwards

				if (e.key.code == Keyboard::Down) 
				// Checks if the key press is down key or not

				{ 
					// Changes the min_im value for traversing Downwards
					// Adds h value from the min_im value
					min_im += h;

					// Changes the max_im value for traversing Downwards
					// Adds h value from the max_im value
					max_im += h; 
				}

			}
			
			//Set Iteration level by mouse wheel
			//the more iteration level the better image result

			// Checking the mouse scrolling event
			if (e.type == Event::MouseWheelScrolled)
			// Event type checker
			{
				if (e.MouseWheelScrolled)
				// Checks if the event of the mouse scrolling happens

				{
					
					if (e.mouseWheelScroll.wheel == Mouse::VerticalWheel)
					{
						
						// Increament of the maximum itteration on the rendered window on downward scrolling
						// a fixed constant of 2 is fixed here
						if (e.mouseWheelScroll.delta > 0)
						max_iter *= 2;
						
						// Reduction of the maximum itteration on the rendered window on upward scrolling
						// a fixed constant of 2 is fixed here
						else 
						max_iter /= 2;
						
						// Changing the max_iter value if it is less than 1 
						// Setting it to 1
						if (max_iter < 1)
						max_iter = 1;

					}
				}
			}


			// Mouse Button pressing event handler
			if (e.type == Event::MouseButtonPressed)
		
			{

				auto zoom_x = [&](double z)
				{
					//mouse point will be new center point

					double cr = min_re + (max_re - min_re)*e.mouseButton.x / W;
					double ci = min_im + (max_im - min_im)*e.mouseButton.y / H;

					//zooms into the center as the selected point as per the mouse pointer
					// Changing the max_re and the min_re values

					double tminr = cr - (max_re - min_re) / 2 / z;
					max_re = cr + (max_re - min_re) / 2 / z;
					min_re = tminr;

					// Changing the max_im and the min_im values

					double tmini = ci - (max_im - min_im) / 2 / z;
					max_im = ci + (max_im - min_im) / 2 / z;
					min_im = tmini;

				};


				//Left Click to ZoomIn
				//Zooms upto a maximum limit i.e. the Max_iterations


				if (e.mouseButton.button == Mouse::Left)
				//checking if the mouse click is right click or not
				//if click is left click then the following is executed

				{
					// Zooms in from the current position
					// The Zoom factor is set to be 5

					zoom_x(5);
					zoom *= 5;
				}


				//Right Click to ZoomOut
				//Zooms back from the zoomed in state


				if (e.mouseButton.button == Mouse::Right)
				//checking if the mouse click is right click or not
				//if click is right click then the following is executed

				{
					// Zooms out from the current position
					// The Zoom factor is set to be 5
					
					zoom_x(1.0 / 5);
					zoom /= 5;

				}
			}
		}



		// window clearing event once simulation is over
		window.clear();

#pragma omp parallel for


		// The loop is a continous coloring pattern that shows the colors on the screen
		/* 
			The primary colors that are used for the simulation are red and black
			On changing the resolution the color decomposes to blue and green 
		*/


		for (int y = 0; y < H; y++)for (int x = 0; x < W; x++)
		{
			// real part declared
			double cr = min_re + (max_re - min_re)*x / W;
			
			// imaginary part declared
			double ci = min_im + (max_im - min_im)*y / H;
			
			// Initialization of the REAL and IMAGINARY part
			double re = 0, im = 0;
			
			// Initialization of looping variable
			int iter;

			for (iter = 0; iter < max_iter; iter++)
			{
				// declare tr and assigning values
				double tr = re * re - im * im + cr;
				im = 2 * re*im + ci;
				
				// Updating variable re to tr
				re = tr;

				// condition for stopping criterion
				if (re*re + im * im > 2 * 2) 
				break;

			}

			int r = 1.0*(max_iter - iter) / max_iter * 0xff;
			int g = r, b = r;

			/*
			//color pallet similar to Ultra Fractal and Wikipedia
			static const vector<Color> colors{
				{0,7,100},
				{32,107,203},
				{237,255,255},
				{255,170,0},
				{0,2,0},
			};
			*/

			// The color codes that are to be plotted on the Mandelbrot Set
			// The color codes range from 0 to 255 inclusive

			static const vector<Color> colors{
				{0,0,0},
				{213,67,31},
				{251,255,121},
				{62,223,89},
				{43,30,218},
				{0,255,247}
			};

			static const auto max_color = colors.size() - 1;
			
			// Limiting condition
			// if the iter becomes equal to max_iter then set the maximum iteration to 0

			if (iter == max_iter)
			iter = 0;
			double mu = 1.0*iter / max_iter;

			//scale mu to be in the range of colors
			
			mu *= max_color;
			
			// casting into a dictionary to represent the colors
			auto i_mu = static_cast<size_t>(mu);

			// the below 2 lines are used for the interpolation of the colors in the rendered screen
			auto color1 = colors[i_mu];
			auto color2 = colors[min(i_mu + 1, max_color)];

			// This leads to the smooth interpolation of the colors in the RenderWindow
			// Zooming changes the colors due to interpolation
			// Zoom in leads to the color being more blue and green
			// Zoom out leads to the color being more Black and Red

			Color c = linear_interpolation(color1, color2, mu - i_mu);


			image.setPixel(x, y, Color(c));
		}
		
		// Coloring technique using the texture,sprite and window objects

		texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);

		// At the left corner of the screen two counters are placed to show the maximum itterations and the Zoom
		char str[100];

		// sprintf is used to print the parameters that are to be printed at one side of the screen
		// The parameters are very helpful for the user 

		sprintf(str, "max iter:%d\nzoom:x%2.2lf", max_iter, zoom);
		text.setString(str);
		window.draw(text);


		// Displaying the window 
		window.display();
	
	}

	return 0;

}