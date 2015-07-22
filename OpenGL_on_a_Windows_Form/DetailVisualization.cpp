#include "stdafx.h"
#include "DetailVisualization.h"

namespace OpenGLForm{
	DetailVisualization::DetailVisualization(Form ^ parentForm,Panel ^ parentPanel, GLsizei iWidth, GLsizei iHeight,ReadCSV read_csv_ref,Preprocessing_Data preprocessing_data_ref):VisualizationPanel(parentForm,parentPanel,iWidth,iHeight,read_csv_ref,preprocessing_data_ref){
			parentPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &DetailVisualization::DetailMouseDown);
			parentPanel->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler( this, &DetailVisualization::DetailMouseWheel );
			parentPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler( this, &DetailVisualization::DetailMouseMove );	
			parentPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &DetailVisualization::DetailMouseUp );				
			title_string();
			//Initialize mouse handler variable
			scale_x[2] = 0.0; scale_y[2] = 0.0; scale_z[2] = 0.0;
			scale_size[2] = 0.05;
			move_x[2] = 0.0; move_y[2] = 0.0; move_z[2] = 0.0;
			scale_factor[2] = 0.6;
			//Initialize window size
			windowWidth[2] = iWidth; 
			windowHeight[2] = iHeight;	

			//time_string();
	}
	System::Void DetailVisualization::Render(int detail_width,int detial_height){
			wglmakecur();

			windowWidth[2] = detail_width;
			windowHeight[2] = detial_height;

			glClearColor(0.0, 0.0, 0.0, 0.0);  //Set the cleared screen colour to black
			glViewport(0, 0, windowWidth[2], windowHeight[2]);   //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window		
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, windowWidth[2], windowHeight[2], 0, -10, 10);

			//Back to the modelview so we can draw stuff 
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer

			glTranslatef(100.0+move_x[2],10.0+move_y[2],0.0+move_z[2]);
			glScalef(scale_factor[2]+scale_x[2]-0.3,scale_factor[2]+scale_y[2],scale_factor[2]+scale_z[2]);
			//vector<float> color;
			//color.resize(3);


			DrawTitle_FTGL(1,100,10);
			DrawCircle(0,0, 6.0, 1, 0, 0);
			DrawTitle_FTGL(2,900,10);
			DrawCircle(800,0, 6.0, 0, 1, 0);
			DrawTitle_FTGL(3,1500,10);
			DrawCircle(1400,0, 6.0, 0, 0, 1);

			if(!histogram_index.empty())
			{
				float y_position = 300;
				float x_position = 10.0;
				int y_position_text = 0;
				for(int u=0;u<histogram_index.size();u++)
				{
					int idx = histogram_index[u];
					//int this_month = preprocessing_data.find_month_and_day[idx].at<int>(0,0);
					//int this_day = preprocessing_data.find_month_and_day[idx].at<int>(0,1);

					//DrawText_FTGL(this_month+1,-50,y_position_text,20.0);
					//DrawText_FTGL(this_day+1,-35,y_position_text,20.0);
					//y_position_text+=10000;

					//for(int j=720000;j<72000+600;j++)
					//{
					//	glPushMatrix(); 
					//	glBegin(GL_QUADS); 
					//		float hour_data_current = preprocessing_data.raw_data_mat.at<float>(j,1)*10.0;
					//		float hour_data_next = preprocessing_data.raw_data_mat.at<float>(j+1,1)*10.0;
					//		glColor3f(0.0, 1.0, 1.0);
					//		glVertex3f(j*15,y_position,0);
					//		glVertex3f((j+1)*15,y_position,0);
					//		glVertex3f((j+1)*15,y_position-hour_data_next,0); 
					//		glVertex3f(j*15,y_position-hour_data_current,0);						
					//	glEnd();
					//	glPopMatrix();
					//}

					//glPushMatrix(); 
					//glBegin(GL_QUADS); 
					//	hour_data_current = preprocessing_data.raw_data_mat.at<float>(1,1)*10.0;
					//	hour_data_next = preprocessing_data.raw_data_mat.at<float>(2,1)*10.0;
					//	glColor3f(1.0, 1.0, 0.0);
					//	glVertex3f(15,y_position,0);
					//	glVertex3f(30,y_position,0);
					//	glVertex3f(30,y_position-hour_data_next,0); 
					//	glVertex3f(15,y_position-hour_data_current,0);						
					//glEnd();
					//glPopMatrix();

					DrawTime_FTGL(histogram_index[u]/600, -150, y_position-20);

					int x_idx = 0;
					for(int i=histogram_index[u]; i<histogram_index[u]+600 ;i++)
					{
						//if(preprocessing_data.data_dim>=3)
						//{
							double hour_data_current = preprocessing_data.raw_data_mat.at<float>(i,1)
												    + preprocessing_data.raw_data_mat.at<float>(i,2)
												    + preprocessing_data.raw_data_mat.at<float>(i,3);
							double hour_data_next = preprocessing_data.raw_data_mat.at<float>(i+1,1)
											      + preprocessing_data.raw_data_mat.at<float>(i+1,2)
											      + preprocessing_data.raw_data_mat.at<float>(i+1,3);
							hour_data_current*=10.0;
							hour_data_next*=10.0;
							//System::Windows::Forms::MessageBox::Show( hour_data_current + " " + hour_data_next);
							//int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[0]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[1]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[2];
							//int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[0]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[1]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[2];
							glPushMatrix(); 
							glBegin(GL_QUADS); 
								//glColor3f(preprocessing_data.raw_data_3D.at<float>(i,0), preprocessing_data.raw_data_3D.at<float>(i,1), preprocessing_data.raw_data_3D.at<float>(i,2)); 
								glColor3f(0.0, 0.0, 1.0);
								glVertex3f(x_idx*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position-hour_data_next,0); 
								glVertex3f(x_idx*5,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();
						//}
							x_idx++;
					}
					
					x_idx = 0;
					for(int i=histogram_index[u]; i<histogram_index[u]+600 ;i++)
					{
						//if(preprocessing_data.data_dim>=2)
						//{
							double hour_data_current = preprocessing_data.raw_data_mat.at<float>(i,1)
												    + preprocessing_data.raw_data_mat.at<float>(i,2);
							double hour_data_next = preprocessing_data.raw_data_mat.at<float>(i+1,1)
											     + preprocessing_data.raw_data_mat.at<float>(i+1,2);
							hour_data_current*=10.0;
							hour_data_next*=10.0;
							//int hour_data_current = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[0]
							//					  + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i].data[1];
							//int hour_data_next = preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[0]
							//				   + preprocessing_data.month_vec[this_month].day_vec[this_day].hour_vec[i+1].data[1];
							glPushMatrix(); 
							glBegin(GL_QUADS); 
								//glColor3f(preprocessing_data.data_color[1][0],preprocessing_data.data_color[1][1],preprocessing_data.data_color[1][2]); 
								glColor3f(0.0, 1.0, 0.0);
								glVertex3f(x_idx*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position-hour_data_next,0); 
								glVertex3f(x_idx*5,y_position-hour_data_current,0);							
							glEnd();
							glPopMatrix();

							x_idx++;
						//}
					}

					x_idx = 0;
					for(int i=histogram_index[u]; i<histogram_index[u]+600 ;i++)
					{
						//if(preprocessing_data.data_dim>=1)
						//{
							//DrawText_FTGL(i,i*15-5,y_position+750,12.0);
							double hour_data_current= preprocessing_data.raw_data_mat.at<float>(i,1);
							double hour_data_next = preprocessing_data.raw_data_mat.at<float>(i+1,1);
							hour_data_current*=10.0;
							hour_data_next*=10.0;
							glPushMatrix(); 
							glBegin(GL_QUADS); 
								//glColor3f(preprocessing_data.data_color[0][0],preprocessing_data.data_color[0][1],preprocessing_data.data_color[0][2]); 
								glColor3f(1.0, 0.0, 0.0); 
								glVertex3f(x_idx*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position,0);
								glVertex3f((x_idx+1)*5,y_position-hour_data_next,0); 
								glVertex3f(x_idx*5,y_position-hour_data_current,0);						
							glEnd();
							glPopMatrix();

							x_idx++;
						//}
					}
					
					//DrawText_FTGL(23,23*15-5,y_position+750,12.0);

					y_position+= 230;
				}
			}	
			//DrawLine(0,480,windowWidth[2]*2,480);
			//DrawLine(450,0,450,1000);
			/*
			if(!raw_data_index.empty())
			{
					int index = raw_data_index[0];
					float factor_g = 10.0;
					int x_position,y_position;
					//==========Gravity X Y Z==========//
					DrawTitle_FTGL(2,70,50);
					x_position = 70;
					y_position = 350;
					for(int j=0;j<raw_data_index.size();j++)
					{
						RECTANGLE *rect;
						index = raw_data_index[j];
						rect = new RECTANGLE();
						rect->h = factor_g*abs(preprocessing_data.raw_data_mat.at<float>(index,0));
						rect->w = 50.0;
						rect->x = x_position;
						rect->y = y_position;
						color[0] = preprocessing_data.raw_data_3D.at<float>(index,0);
						color[1] = preprocessing_data.raw_data_3D.at<float>(index,1);
						color[2] = preprocessing_data.raw_data_3D.at<float>(index,2);
						if(preprocessing_data.raw_data_mat.at<float>(index,0)>0)
						{
							rect->y -= factor_g*abs(preprocessing_data.raw_data_mat.at<float>(index,0));
							float current_position_y = rect->y;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,0));
							DrawText_FTGL(nearest,x_position,current_position_y-20);
						}
						else
						{
							float current_position_y = rect->y + rect->h;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,0));
							DrawText_FTGL(nearest,x_position,current_position_y+5);
						}

						DrawRectWithOpenGL(rect,color);
						delete(rect);

						x_position+=50;
					}
					x_position+=70;
					//==========Linear Acceleration X Y Z==========//
					DrawTitle_FTGL(1,750,50);
					x_position = 750;
					y_position = 350;
					int factor_la = 10;
					for(int j=0;j<raw_data_index.size();j++)
					{
						index = raw_data_index[j];
						RECTANGLE *rect;
						rect = new RECTANGLE();
						rect->h = factor_la*abs(preprocessing_data.raw_data_mat.at<float>(index,1));
						rect->w = 50.0;
						rect->x = x_position;
						rect->y = y_position;
						color[0] = preprocessing_data.raw_data_3D.at<float>(index,0);
						color[1] = preprocessing_data.raw_data_3D.at<float>(index,1);
						color[2] = preprocessing_data.raw_data_3D.at<float>(index,2);
						if(preprocessing_data.raw_data_mat.at<float>(index,1)>0)
						{
							rect->y -= factor_la*abs(preprocessing_data.raw_data_mat.at<float>(index,1));
							float current_position_y = rect->y;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,1));
							DrawText_FTGL(nearest,x_position-10,current_position_y-20);
						} 
						else
						{
							float current_position_y = rect->y + rect->h;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,1));
							DrawText_FTGL(nearest,x_position-10,current_position_y+5);
						}

						DrawRectWithOpenGL(rect,color);
						delete(rect);

						x_position+=50;
					}
					x_position+=100;
					//==========Gyroscope X Y Z==========//
					DrawTitle_FTGL(3,70, 450);
					x_position = 70;
					y_position = 850;
					int factor2 = 10;
					for(int j=0;j<raw_data_index.size();j++)
					{
						index = raw_data_index[j];
						RECTANGLE *rect;
						rect = new RECTANGLE();
						rect->h = factor2*abs(preprocessing_data.raw_data_mat.at<float>(index,2));
						rect->w = 50.0;
						rect->x = x_position;
						rect->y = y_position;
						color[0] = preprocessing_data.raw_data_3D.at<float>(index,0);
						color[1] = preprocessing_data.raw_data_3D.at<float>(index,1);
						color[2] = preprocessing_data.raw_data_3D.at<float>(index,2);
						if(preprocessing_data.raw_data_mat.at<float>(index,2)>0)
						{
							rect->y -= factor2*abs(preprocessing_data.raw_data_mat.at<float>(index,2));
							float current_position_y = rect->y;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,2));
							DrawText_FTGL(nearest,x_position,current_position_y-20);
						}
						else
						{
							float current_position_y = rect->y + rect->h;
							double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,2));
							DrawText_FTGL(nearest,x_position,current_position_y+5);
						}

						DrawRectWithOpenGL(rect,color);
						delete(rect);

						x_position+=50.0;
					}
					x_position+=100;
					//==========first order of Longitude & Latitude==========//
					DrawTitle_FTGL(0,750,450);
					x_position = 750;
					y_position = 850;	
						
					int factor3 = 100.0;
					for(int j=0;j<raw_data_index.size();j++)
					{							
						RECTANGLE *rect;
						index = raw_data_index[j];
						float value = abs(preprocessing_data.raw_data_mat.at<float>(index,3));
						float adj_value = value/10.0;
						//if(value>1.0)
						//	adj_value = 1.0;
						rect = new RECTANGLE();
						rect->h = factor3*adj_value;
						rect->w = 50.0;
						rect->x = x_position;
						rect->y = y_position;
						color[0] = preprocessing_data.raw_data_3D.at<float>(index,0);
						color[1] = preprocessing_data.raw_data_3D.at<float>(index,1);
						color[2] = preprocessing_data.raw_data_3D.at<float>(index,2);
						if(adj_value>0.0)
						{
							rect->y -= factor3*adj_value;
							float current_position_y = rect->y;
							double nearest = round(adj_value);
							DrawText_FTGL(nearest,x_position,current_position_y-20);
						}
						else if(adj_value==0.0)
						{
							float current_position_y = rect->y;
							DrawText_FTGL(0,x_position,current_position_y-20);
						}
						else //not enter
						{
							float current_position_y = rect->y + rect->h;
							//double nearest = round(preprocessing_data.raw_data_mat.at<float>(index,3));
							DrawText_FTGL(preprocessing_data.raw_data_mat.at<float>(index,3),x_position,current_position_y+5);
						}

						DrawRectWithOpenGL(rect,color);
						delete(rect);

						x_position+=50;
					}
					x_position+=70;		
			}*/
		
			SwapOpenGLBuffers();
			
		}
		 
		System::Void DetailVisualization::DrawText_FTGL(float n,int x, int y)
		{
			glPushMatrix();

			float font_size = 10*(scale_factor[2]+0.4+scale_x[2]);
			font.FaceSize(font_size);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(x , y + font.LineHeight());
			stringstream ss;
			ss << n;
			string str = ss.str();
			text = (char*)str.c_str();
			font.Render(text);

			glPopMatrix();
		
		}

		System::Void DetailVisualization::DrawTitle_FTGL(int t,int x, int y)
		{
			glPushMatrix();

			float font_size = 20*(scale_factor[2]+0.4+scale_x[2]);
			font.FaceSize(font_size);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(x , y + font.LineHeight());
			font.Render(title[t]);

			glPopMatrix();			
		}

		System::Void DetailVisualization::title_string()
		{
			strcpy(title[0],"Gravity");
			strcpy(title[1],"Linear Acceleration");
			strcpy(title[2],"Gyroscope");
			strcpy(title[3],"First Order of Distance");
		}

		System::Void DetailVisualization::DrawLine(int x1,int y1,int x2,int y2)
		{
			glPushMatrix();
			glLineWidth(2.0);
			glBegin(GL_LINES);
				glColor3f(1.0,1.0,1.0);
				glVertex3f(x1,y1,0.0);
				glVertex3f(x2,y2,0.0);
			glEnd();
			glPopMatrix();	
		}

	System::Void DetailVisualization::DetailMouseDown( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			last_X[2] = e->X;
			last_Y[2] = e->Y;

			if (e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				//FindPatternByTable2(e->X,e->Y);
			}
	}
	System::Void DetailVisualization::DetailMouseWheel( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			
			if (e->Delta < 0){
				scale_x[2]+=scale_size[2];
				scale_y[2]+=scale_size[2];
				scale_z[2]+=scale_size[2];
			}
			else{
				scale_x[2]-=scale_size[2];
				scale_y[2]-=scale_size[2];
				scale_z[2]-=scale_size[2];		
			}
			
			
	}
	System::Void DetailVisualization::DetailMouseMove( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			if (e->Button == System::Windows::Forms::MouseButtons::Left)
			{
					vector2 Move(e->X - last_X[2] , e->Y - last_Y[2]);
					if (Move.length() < 500.0f)
					{
						move_x[2] += 0.3*Move.x;
						move_y[2] += 0.3*Move.y;
					}
			}		
	}
	System::Void DetailVisualization::DetailMouseUp( Object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e ){
			last_X[2] = e->X;
			last_Y[2] = e->Y;		
	}

		System::Void DetailVisualization::clear()
		{
			raw_data_index.clear();
		}
}