#include "stdafx.h"
#include "VisualizationPanel.h"

namespace OpenGLForm{
	ReadCSV read_csv;
	Preprocessing_Data preprocessing_data;
	char quote[24][80];
	char title[4][80];
	char five_minutes[12][80];
	vector<vector4> histogram_position_table;
	vector<int> histogram_index;
	vector< vector<vector4> > raw_data_position_table;
	vector<int> raw_data_index;
	float scale_x[3],scale_y[3],scale_z[3];
	float scale_size[3],scale_factor[3];
	int last_X[3],last_Y[3];
	float move_x[3],move_y[3],move_z[3];
	int windowWidth[3],windowHeight[3];
	FTPixmapFont font(FONT_FILE);
	char* text;
	double ModelViewMatrix2[16];
	int select_histogram_index;
	bool select_histogram_flag;
	int counter;

	VisualizationPanel::VisualizationPanel(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref){
			CreateParams^ cp = gcnew CreateParams;	
			read_csv = read_csv_ref;
			preprocessing_data = preprocessing_data_ref;
			// Set the position on the form
			cp->X = 0;
			cp->Y = 0;
			cp->Height = iHeight;
			cp->Width = iWidth;

			// Specify the form as the parent.
			cp->Parent = parentForm->Handle;

			// Create as a child of the specified parent and make OpenGL compliant (no clipping)
			cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			// Create the actual window
			this->CreateHandle(cp);
			//parentForm->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &COpenGL::COGL_MouseClick);
			//parentForm->MouseClick += gcnew System::Windows::Forms::MouseEventHandler( this, &COpenGL::panel1_MouseDown );
			//parentForm->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler( this, &COpenGL::COGL_Scroll );
			m_hDC = GetDC((HWND)parentPanel->Handle.ToPointer());

			this->parentForm = parentForm;/////////////////////////////

			if(m_hDC)
			{
				MySetPixelFormat(m_hDC);
				ReSizeGLScene(iWidth, iHeight);
				InitGL();		
			}

			rtri = 0.0f;
			rquad = 0.0f;

			time_string();

	}

	VisualizationPanel::~VisualizationPanel(System::Void){
		this->DestroyHandle();
	}


	GLint VisualizationPanel::MySetPixelFormat(HDC hdc){
		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
				{
					sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
					1,											// Version Number
					PFD_DRAW_TO_WINDOW |						// Format Must Support Window
					PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
					PFD_DOUBLEBUFFER,							// Must Support Double Buffering
					PFD_TYPE_RGBA,								// Request An RGBA Format
					16,										// Select Our Color Depth
					0, 0, 0, 0, 0, 0,							// Color Bits Ignored
					0,											// No Alpha Buffer
					0,											// Shift Bit Ignored
					0,											// No Accumulation Buffer
					0, 0, 0, 0,									// Accumulation Bits Ignored
					16,											// 16Bit Z-Buffer (Depth Buffer)  
					0,											// No Stencil Buffer
					0,											// No Auxiliary Buffer
					PFD_MAIN_PLANE,								// Main Drawing Layer
					0,											// Reserved
					0, 0, 0										// Layer Masks Ignored
				};
			
			GLint  iPixelFormat; 
		 
			// get the device context's best, available pixel format match 
			if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
			{
				MessageBox::Show("ChoosePixelFormat Failed");
				return 0;
			}
			 
			// make that match the device context's current pixel format 
			if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
			{
				MessageBox::Show("SetPixelFormat Failed");
				return 0;
			}

			if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
			{
				MessageBox::Show("wglCreateContext Failed");
				return 0;
			}

			if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
			{
				MessageBox::Show("wglMakeCurrent Failed");
				return 0;
			}


			return 1;
	}
	bool VisualizationPanel::InitGL(GLvoid){
		glShadeModel(GL_SMOOTH);							// Enable smooth shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
		glClearDepth(1.0f);									// Depth buffer setup
		glEnable(GL_DEPTH_TEST);							// Enables depth testing
		glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
		return TRUE;										// Initialisation went ok
	}
	GLvoid VisualizationPanel::ReSizeGLScene(GLsizei width, GLsizei height){
			if (height==0)										// Prevent A Divide By Zero By
			{
				height=1;										// Making Height Equal One
			}

			glViewport(0,0,width,height);						// Reset The Current Viewport

			glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
			glLoadIdentity();									// Reset The Projection Matrix

			// Calculate The Aspect Ratio Of The Window
			gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

			glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
			glLoadIdentity();									// Reset The Modelview Matrix
	}
	System::Void VisualizationPanel::DrawRectWithOpenGL(RECTANGLE* pRect,vector<float> draw_color){
		glPushMatrix();  //Make sure our transformations don't affect any other transformations in other code
		//glTranslatef(pRect->x, pRect->y,0);  //Translate rectangle to its assigned x and y position
			
		//glBegin(GL_TRIANGLES);
		glBegin(GL_QUADS);   //We want to draw a quad, i.e. shape with four sides
			glColor3f(draw_color[0],draw_color[1],draw_color[2]); //Set the colour to red 
			glVertex3f(pRect->x,pRect->y,0);
			glVertex3f(pRect->x+pRect->w,pRect->y,0);
			glVertex3f(pRect->x+pRect->w,pRect->y+pRect->h,0);
			glVertex3f(pRect->x,pRect->y+pRect->h,0);      
		glEnd();
		glPopMatrix();
	}

	System::Void VisualizationPanel::DrawText_FTGL(int n,int x, int y, float scale)
	{
		glPushMatrix();

		glTranslatef(x, y-30, 0);
		glScalef(1.0+scale_x[1],1.0+scale_y[1],1.0+scale_z[1]);	
	
		//float font_size = 10*(scale_factor[2]+0.4+scale_x[2]);	
		font.FaceSize(scale);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(0 , 0 + font.LineHeight());
		stringstream ss;
		ss << n;
		string str = ss.str();
		char zero[] = "0";
		if(n<10)
		{
			strcat(zero,str.c_str());
			font.Render(zero);
		}
		else
		{
			char* text = (char*)str.c_str();
			font.Render(text);
		}

		glPopMatrix();
		
	}

	System::Void VisualizationPanel::DrawTime_FTGL(int index,int x, int y)
	{
		int hour;
		int five_minute_index;
		for(int i=0;i<preprocessing_data.hour_range.size();i++)
		{
			if(index>=preprocessing_data.hour_range[i].x && index<= preprocessing_data.hour_range[i].y)
			{
				hour = preprocessing_data.hour_index[i];
				if(hour == preprocessing_data.begin_hour && preprocessing_data.num_of_begin_hour<12)
					five_minute_index = index - preprocessing_data.hour_range[i].x + (12-preprocessing_data.num_of_begin_hour);
				else
					five_minute_index = index - preprocessing_data.hour_range[i].x;
				
				break;
			}
		}
		stringstream ss;
		ss << hour;
		string str = ss.str();
		char *hour_text = (char*)str.c_str();		

		strcat(hour_text,five_minutes[five_minute_index]);

		glPushMatrix();

		float font_size = 10*(scale_factor[2]+0.8+scale_x[2]);
		font.FaceSize(font_size);
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(x , y-20.0 + font.LineHeight());
		font.Render(hour_text);

		glPopMatrix();		
	}

	System::Void VisualizationPanel::DrawCircle(int x, int y, float radius, float r, float g, float b)
	{
		int i;
		int triangleAmount = 30; //# of triangles used to draw circle
	
		//GLfloat radius = 0.8f; //radius
		GLfloat twicePi = 2.0f * 3.14;
	
		glBegin(GL_TRIANGLE_FAN);
			glColor3f(r,g,b);
			glVertex2f(x, y); // center of circle
			for(i = 0; i <= triangleAmount;i++)
			{ 
				glVertex2f(
					x + (radius * cos(i *  twicePi / triangleAmount)), 
					y + (radius * sin(i * twicePi / triangleAmount))
				);
			}
		glEnd();		
	}

	System::Void VisualizationPanel::SwapOpenGLBuffers(System::Void){
		SwapBuffers(m_hDC);		
	}
	System::Void VisualizationPanel::wglmakecur(){
		wglMakeCurrent(m_hDC,m_hglrc);
	}

	double VisualizationPanel::round(double f)
	{
		int pp = 2;
		return int(f * pow(10.0, pp) + .5) /  pow(10.0, pp);
	}

	System::Void VisualizationPanel::time_string()
	{
		strcpy(five_minutes[0],":00");
		strcpy(five_minutes[1],":05");
		strcpy(five_minutes[2],":10");
		strcpy(five_minutes[3],":15");
		strcpy(five_minutes[4],":20");
		strcpy(five_minutes[5],":25");
		strcpy(five_minutes[6],":30");
		strcpy(five_minutes[7],":35");
		strcpy(five_minutes[8],":40");
		strcpy(five_minutes[9],":45");
		strcpy(five_minutes[10],":50");
		strcpy(five_minutes[11],":55");
	}
}