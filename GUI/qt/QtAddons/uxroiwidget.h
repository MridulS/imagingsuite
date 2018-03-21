#ifndef UXROIWIDGET_H
#define UXROIWIDGET_H

#include <QWidget>
#include <QRect>
#include <QString>

#include <logging/logger.h>

namespace Ui {
class uxROIWidget;
}

namespace QtAddons {

class ImageViewerWidget;
/// \brief The widget provides the ability to handle ROIs in imageviewers.
///
/// There is a tight connection with the ImageViewerWidget that allow to visualize ROIs and to obtain image dimensions.
class uxROIWidget : public QWidget
{
    Q_OBJECT
    kipl::logging::Logger logger;
    static int cnt; //< Counter for id numbers
    int roiID; //< The ID of the current ROI widget. This will be used to reference the ROI in the ImageViewer.
public:
    explicit uxROIWidget(QWidget *parent = 0);
    ~uxROIWidget();

    /// \brief Set the ROI coordinates using two x/y pairs
    /// \param x0,y0 First corner
    /// \param x1,y1 Second corner
    void setROI(int x0, int y0, int x1, int y1);

    /// \brief Set the ROI coordinates using an array with two x/y pairs
    /// \param roi Coordinates organized as x0,y0,x1,y1.
    void setROI(int *roi);

    /// \brief Set the ROI coordinates using a Qt rect
    /// \param rect The rectangle object describing the roi
    void setROI(QRect rect);

    /// \brief Sets the bounding box limiting the ROI
    /// \param x0,y0 First corner
    /// \param x1,y1 Second corner
    /// \param updateFields Updates the roi coordinates to the bounding box.
    void setBoundingBox(int x0, int y0, int x1, int y1, bool updateFields=false);

    /// \brief Returns QRect describing the ROI
    void getROI(QRect &rect);

    /// \brief Returns x0,y0,x1,y1 describing the oposite corners of the ROI
    void getROI(int &x0, int &y0, int &x1, int &y1);

    /// \brief Popultes a four element array with x0,y0,x1,y1 describing the oposite corners of the ROI
    void getROI(int *roi);

    /// \brief Set the title of the ROI widget frame
    /// \param lbl The name string
    void setTitle(const QString &lbl);

    /// \brief Set color of the ROI. This color will be shared between widget and the region marked in the viewer.
    /// \param color A string containing a valid Qt color name (https://www.w3.org/TR/SVG/types.html#ColorKeywords).
    bool setROIColor(const QString color);


    /// \brief Registers a handle to the viewer which will be used for selection and visualization of ROIs.
    /// \param viewer A pointer to the viewer widget.
    void registerViewer(ImageViewerWidget *viewer);

public slots:
    /// \brief A slot the responds to the NewImage signal emitted by the viewer widget. It is used to update the bounding box of the ROI widget.
    void on_viewerNewImageDims(const QRect &rect);

private slots:
    void on_spinX0_valueChanged(int arg1); //< Slot for changed X0 value

    void on_spinY0_valueChanged(int arg1); //< Slot for changed Y0 value

    void on_spinY1_valueChanged(int arg1); //< Slot for changed X1 value

    void on_spinX1_valueChanged(int arg1); //< Slot for changed Y1 value

    /// \brief Slot to handle the get roi from viewer signal
    void on_buttonGetROI_clicked();

    /// \brief Slot to update the value boxes when a spin button value is changed
    void on_valueChanged(int x0,int y0, int x1, int y1);

private:
    /// \brief Updates the roi rectangle in the viewer
    void updateViewer();

    /// \brief Updates the max boundaries of the spin buttons to the size of the current image in the viewer
    void updateBounds();

    Ui::uxROIWidget *ui;

    ImageViewerWidget * hViewer;
    QString roiColor;

signals:
    void getROIClicked(void); //< signal emitted when the get ROI button is pressen, this can be used with other widgets.
    void valueChanged(int x0,int y0, int x1, int y1); // signal emitted when one of the spin buttons change value.
};

}
#endif // UXROIWIDGET_H
