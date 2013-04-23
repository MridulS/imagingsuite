#include "imagepainter.h"
#include <math/image_statistics.h>

namespace QtAddons {

ImagePainter::ImagePainter() :
          //m_Interpolation(Gdk::INTERP_TILES),
          m_data(NULL)
         //m_cdata(NULL)
{
      m_dims[0]=m_dims[1]=16;
      m_NData=m_dims[0]*m_dims[1];
      m_data=new float[m_NData];

      for (int i=0; i<m_NData; i++)
          m_data[i]=static_cast<float>(i);
      m_MinVal = 0.0f;
      m_MaxVal = static_cast<float>(m_NData);

      prepare_pixbuf();
}

ImagePainter::~ImagePainter()
{
}

void ImagePainter::Render(const QPainter &painter, int x, int y, int w, int h)
{
    float img_ratio        = static_cast<float>(m_dims[0])/static_cast<float>(m_dims[1]);
    float allocation_ratio = static_cast<float>(w)/static_cast<float>(h);

//    Glib::RefPtr<Gdk::Pixbuf> scaled;

//    if (img_ratio < allocation_ratio)
//        scaled=m_pixbuf_full->scale_simple(static_cast<int>(h*img_ratio),
//                                     h,
//                                     m_Interpolation);
//    else
//        scaled=m_pixbuf_full->scale_simple(w,
//                                     static_cast<int>(w/img_ratio),
//                                     m_Interpolation);

//    offset_x = (w-scaled->get_width())/2+x;
//    offset_y = (h-scaled->get_height())/2+y;

//    scaled_width = scaled->get_width();
//    scaled_height = scaled->get_height();

//    Cairo::Format format = Cairo::FORMAT_RGB24;

//    Cairo::RefPtr< Cairo::ImageSurface > image_surface_ptr = Cairo::ImageSurface::create  (format, scaled->get_width(), scaled->get_height());

//    // Create the new Context for the ImageSurface
//    Cairo::RefPtr< Cairo::Context > image_context_ptr = Cairo::Context::create (image_surface_ptr);

//    Gdk::Cairo::set_source_pixbuf (image_context_ptr, scaled, 0, 0);
//    image_context_ptr->paint();

//    cr->set_source(image_surface_ptr, offset_x, offset_y);
//    cr->paint();

//    if (!m_BoxList.empty() || !m_PlotList.empty()) {
//        float scale=scaled->get_width()/static_cast<float>(m_dims[0]);

//        cr->set_line_width(1.0);

//        if (!m_BoxList.empty()) {

//            std::map<int,ImageViewerRectangle>::iterator it;

//            for (it=m_BoxList.begin(); it!=m_BoxList.end(); it++) {
//                Gdk::Cairo::set_source_color(cr, it->second.color);
//                cr->move_to(offset_x+it->second.x*scale,offset_y+it->second.y*scale);
//                cr->rel_line_to(it->second.width*scale,0);
//                cr->rel_line_to(0,it->second.height*scale);
//                cr->rel_line_to(-it->second.width*scale,0);
//                cr->rel_line_to(0,-it->second.height*scale);

//                cr->stroke();
//            }
//        }

//        if (!m_PlotList.empty()) {
//            std::map<int,PlotData>::iterator it;

//            for (it=m_PlotList.begin(); it!=m_PlotList.end(); it++) {

//                Gdk::Cairo::set_source_color(cr, it->second.color);

//                cr->move_to(offset_x+it->second.dataX[0]*scale,offset_y+it->second.dataY[0]*scale);
//                for (int i=1; i<it->second.size(); i++) {
//                    cr->line_to(offset_x+it->second.dataX[i]*scale,offset_y+it->second.dataY[i]*scale);
//                }
//                cr->stroke();
//            }
//        }
//    }
//    Gdk::Cairo::set_source_color(cr, Gdk::Color("black"));
}

void ImagePainter::set_image(float const * const data, size_t const * const dims)
{
    if (m_data!=NULL) {
        delete [] m_data;
    }
    m_dims[0]=dims[0];
    m_dims[1]=dims[1];
    m_NData=m_dims[0]*m_dims[1];
    m_data=new float[m_NData];

    memcpy(m_data,data,sizeof(float)*m_NData);

    kipl::math::minmax(m_data,m_NData,&m_ImageMin, &m_ImageMax);
    m_MinVal=m_ImageMin;
    m_MaxVal=m_ImageMax;

 //   m_BoxList.clear();
 //   m_PlotList.clear();

    prepare_pixbuf();
}

void ImagePainter::set_image(float const * const data, size_t const * const dims, const float low, const float high)
{
    if (m_data!=NULL) {
        delete [] m_data;
    }
    m_dims[0]=dims[0];
    m_dims[1]=dims[1];
    m_NData=m_dims[0]*m_dims[1];
    m_data=new float[m_NData];

    memcpy(m_data,data,sizeof(float)*m_NData);
    kipl::math::minmax(m_data,m_NData,&m_ImageMin, &m_ImageMax);
    m_MinVal=low;
    m_MaxVal=high;

 //   m_BoxList.clear();
 //   m_PlotList.clear();
    prepare_pixbuf();
}

//void ImagePainter::set_plot(PlotData data, int idx)
//{
//    m_PlotList[idx]=data;
//}

//int ImagePainter::clear_plot(int idx)
//{
//    std::map<int,PlotData>::iterator it;

//    if (!m_PlotList.empty()) {
//        if (idx<0)  {
//            m_PlotList.clear();
//        }
//        else {
//            it=m_PlotList.find(idx);
//            if (it!=m_PlotList.end()) {
//                m_PlotList.erase (it);
//            }
//        }
//        return 1;
//    }
//    return 0;
//}

//void ImagePainter::set_rectangle(ImageViewerRectangle rect, int idx)
//{
//    m_BoxList[idx]=rect;
//}

//int ImagePainter::clear_rectangle(int idx)
//{
//    std::map<int,ImageViewerRectangle>::iterator it;

//    if (!m_BoxList.empty()) {
//        if (idx<0) {
//            m_BoxList.clear();
//        }
//        else
//        {
//            it=m_BoxList.find(idx);

//            if (it!=m_BoxList.end()) {
//                m_BoxList.erase (it);
//            }
//        }
//        return 1;
//    }

//    return 0;
//}

//int ImagePainter::clear()
//{
//    m_BoxList.clear();
//    m_PlotList.clear();

//    return 1;
//}

void ImagePainter::set_levels(const float level_low, const float level_high)
{
    m_MinVal = level_low;
    m_MaxVal = level_high;
    prepare_pixbuf();
}

void ImagePainter::get_levels(float *level_low, float *level_high)
{
    *level_low  = m_MinVal;
    *level_high = m_MaxVal;
}

void ImagePainter::get_image_minmax(float *level_low, float *level_high)
{
    *level_low  = m_ImageMin;
    *level_high = m_ImageMax;
}

void ImagePainter::show_clamped(bool show)
{

}

void ImagePainter::prepare_pixbuf()
{
//    if (m_cdata!=NULL)
//        delete [] m_cdata;

//    m_cdata=new guint8[m_NData*3];
//    memset(m_cdata,0,3*sizeof(guint8)*m_NData);

//    const float scale=255.0f/(m_MaxVal-m_MinVal);
//    float val=0.0;
//    int idx=0;
//    for (int i=0; i<m_NData; i++) {
//        idx=3*i;
//        if (m_data[i]==m_data[i]) {
//            val=(m_data[i]-m_MinVal)*scale;
//            if (255.0f<val)
//                m_cdata[idx]=255;
//            else if (val<0.0f)
//                m_cdata[idx]=0;
//            else
//                m_cdata[idx]=static_cast<guint8>(val);

//            m_cdata[idx+1]=m_cdata[idx+2]=m_cdata[idx];
//        }
//        else {
//            m_cdata[idx]=255;
//            m_cdata[idx+1]=0;
//            m_cdata[idx+2]=0;
//        }
//    }

//    if (m_NData!=0)
//        m_pixbuf_full=Gdk::Pixbuf::create_from_data(m_cdata,Gdk::COLORSPACE_RGB,false,8,m_dims[0],m_dims[1],3*m_dims[0]);

}
} /* namespace Gtk_addons */
