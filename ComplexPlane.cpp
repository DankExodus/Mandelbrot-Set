#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;
	m_aspectRatio = m_pixel_size.x / m_pixel_size.y;
	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_State = CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(m_pixel_size.x * m_pixel_size.y);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
	double sizeX = BASE_WIDTH *  pow(BASE_ZOOM, m_zoomCount);
	double sizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = Vector2f(sizeX, sizeY);
	m_zoomCount++;
	m_State == CALCULATING;
}
void ComplexPlane::zoomOut()
{
	double sizeX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	double sizeY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = Vector2f(sizeX, sizeY);
	m_zoomCount--;
	m_State == CALCULATING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
	Vector2f complexcoord = mapPixelToCoords(mousePixel);
	m_plane_center = complexcoord;
	m_State = CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	Vector2f complexcoord = mapPixelToCoords(mousePixel);
	m_mouselocation = complexcoord;
}

void ComplexPlane::loadText(Text& text) 
{
	stringstream string;
	string << "Mandelbrot Set\n";
	string << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
	string << "Cursor: (" << m_mouselocation.x << ", " << m_mouselocation.y << ")\n";
	string << "Left-click to Zoom in\n";
	string << "Right-click to Zoom out\n";

	text.setString(string.str());
	text.setCharacterSize(20);
	text.setPosition(10, 0);
}
void ComplexPlane::updateRender()
{
	
	if (m_State == CALCULATING) {
		for (float j = 0; j < m_pixel_size.x; j++)
		{
			for (float i = 0; i < m_pixel_size.y; i++)
			{
				int index = j + i * m_pixel_size.x;
				m_vArray[index].position = Vector2f(j, i);
				Vector2f complexCoord = mapPixelToCoords(Vector2i(j, i));
				size_t iterations = countIterations(complexCoord);
				Uint8 r, g, b;
				iterationsToRGB(iterations, r, g, b);
				m_vArray[index].color = Color(r, g, b);
			}
		}
	}
}
int ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y);
	complex<double> z = c;

	int i = 0;

	while (abs(z) < 2.0 && i < MAX_ITER)
	{
		z = z * z + c;
		i++;
	}
	return i;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	float t = static_cast<float>(count) / MAX_ITER;
	if (count == MAX_ITER)
	{
		//black
		r = 0, g = 0, b = 0;
	}

	if (t < 0.1)
	{
		//dark blue
		r = 0, g = 4, b = 46;
	}
	else if (t < 0.2)
	{
		//blue
		r = 0,g = 13, b = 155;
	}
	else if (t < 0.3)
	{
		//light blue
		r = 41, g = 59, b = 255;
	}
	else if (t < 0.4)
	{
		//turquoise
		r = 0, g = 155, b = 255;
	}
	else if (t < 0.6)
	{
		//yellow
		r = 255, g = 255,b = 0;
	}
	else if (t < 0.8)
	{
		//red
		r = 255, g = 0, b = 0;
	}

}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	Vector2f complexCoord;
	float real_min = m_plane_center.x - m_plane_size.x / 2.0; 
	float real_max = m_plane_center.x + m_plane_size.x / 2.0; 
	float imag_min = m_plane_center.y - m_plane_size.y / 2.0; 
	float imag_max = m_plane_center.y + m_plane_size.y / 2.0; 

	float x_min = 0.0;                   
	float x_max = m_pixel_size.x; 
	float y_min = 0.0;                   
	float y_max = m_pixel_size.y; 

	float real = ((mousePixel.x - x_min) / (x_max - x_min)) * (real_max - real_min) + real_min;
	
	float imag = ((mousePixel.y - y_min) / (y_max - y_min)) * (imag_max - imag_min) + imag_min;

	complexCoord = Vector2f(real, imag);

	return complexCoord;
}
