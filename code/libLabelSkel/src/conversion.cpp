/**
 * @copyright Copyright (2011) Willow Garage
 * @author Koen Buys
 * @file conversion.cpp
 * @brief this file contains the implementation of all conversion functions
 **/

// our headers
#include "libLabelSkel/blob2.h"
#include "libLabelSkel/common.h"
#include "libLabelSkel/conversion.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace LabelSkel 
{
/**
 * @brief This function takes a cvMat in and a pointcloud and assumes they are both registered
 * It colors the output pointcloud with the colors of the input cvMat
 * @param[in] cmap the openCV matrix to thake the rgb info from
 * @param[in] cloud_in the input pointcloud to take xyz from
 * @param[out] cloud_out the input pointcloud colored with the rgb info from the opencv mat
 * @return this is zero when everything went well
 */
int makeColoredPointCloud(  cv::Mat&                            cmap,
                            pcl::PointCloud<pcl::PointXYZRGB>&  cloud_in,
                            pcl::PointCloud<pcl::PointXYZRGB>&  cloud_out)
{
  // Test if input image and cloud are same size, otherwise this won't work
  assert((int) cloud_in.width == cmap.cols);
  assert((int) cloud_in.height == cmap.rows);

  // Set output size of the cloud_out
  cloud_out.width = cmap.cols;
  cloud_out.height = cmap.rows;

  for(int h = 0; h < cmap.rows; h++)
  {
    for(int w = 0; w < cmap.cols; w++)
    {
      //cv::Point p(w,h);
      //cv::Vec3b& bgr = cmap(p);
      cv::Vec3b& bgr = cmap.at<cv::Vec3b>(h,w);
      pcl::PointXYZRGB point;
      point.r = bgr[2];
      point.g = bgr[1];
      point.b = bgr[0];
      point.x = cloud_in.points[h*cmap.cols + w].x;
      point.y = cloud_in.points[h*cmap.cols + w].y;
      point.z = cloud_in.points[h*cmap.cols + w].z;
      cloud_out.points.push_back(point);
    }
  }
  return 0;
}
/**
 * @brief This function takes a cvMat in and a pointcloud and assumes they are both registered
 * It colors the output pointcloud with the colors of the input cvMat
 * @param[in] cmap the openCV matrix to thake the rgb info from
 * @param[in] cloud_in the input pointcloud to take xyz from
 * @param[out] cloud_out the input pointcloud colored with the rgb info from the opencv mat
 * @return this is zero when everything went well
 */
int makeColoredPointCloud(  cv::Mat&                            cmap,
                            pcl::PointCloud<pcl::PointXYZ>&     cloud_in,
                            pcl::PointCloud<pcl::PointXYZRGB>&  cloud_out)
{
  // Test if input image and cloud are same size, otherwise this won't work
  assert((int) cloud_in.width == cmap.cols);
  assert((int) cloud_in.height == cmap.rows);

  // Set output size of the cloud_out
  cloud_out.width = cmap.cols;
  cloud_out.height = cmap.rows;

  for(int h = 0; h < cmap.rows; h++)
  {
    for(int w = 0; w < cmap.cols; w++)
    {
      //cv::Point p(w,h);
      //cv::Vec3b& bgr = cmap(p);
      cv::Vec3b& bgr = cmap.at<cv::Vec3b>(h,w);
      pcl::PointXYZRGB point;
      point.r = bgr[2];
      point.g = bgr[1];
      point.b = bgr[0];
      point.x = cloud_in.points[h*cmap.cols + w].x;
      point.y = cloud_in.points[h*cmap.cols + w].y;
      point.z = cloud_in.points[h*cmap.cols + w].z;
      cloud_out.points.push_back(point);
    }
  }
  return 0;
}
/**
 * @brief This function generates a image from pointcloud
 * @param[out] image the image to which the data will be written
 * @param[in] cloud the pointcloud from which the RGB data will be taken
 * @return zero if everything went well
 **/
int makeImageFromPointCloud(cv::Mat&                            image,
                            pcl::PointCloud<pcl::PointXYZRGB>&  cloud)
{
  assert(image.cols == (int) cloud.width);
  assert(image.rows == (int) cloud.height);

  // First iterate over the pointcloud
  for(unsigned int h = 0; h < cloud.height; h++){
    for(unsigned int w = 0; w < cloud.width; w++){
      image.at<cv::Vec3b>(h,w)[0] = cloud.points[h*cloud.width + w].b;
      image.at<cv::Vec3b>(h,w)[1] = cloud.points[h*cloud.width + w].g;
      image.at<cv::Vec3b>(h,w)[2] = cloud.points[h*cloud.width + w].r;
    }
  }
  return 0;
}
/**
 * @brief This function generates a image from pointcloud
 * @param[out] image the image to which the data will be written
 * @param[in] indices the point indices that need to be projected
 * @param[in] cloud the pointcloud from which the RGB data will be taken
 * @return zero if everything went well
 **/
int makeImageFromPointCloud(cv::Mat&                            image,
                            pcl::PointIndices                   indices,
                            pcl::PointCloud<pcl::PointXYZRGB>&  cloud)
{
  assert(image.cols == (int) cloud.width);
  assert(image.rows == (int) cloud.height);

  for(unsigned int i = 0; i < indices.indices.size(); i++)
  {
    int index = indices.indices[i];
    unsigned int h = index/cloud.width;
    unsigned int w = index%cloud.width;
    image.at<cv::Vec3b>(h,w)[0] = cloud.points[index].b;
    image.at<cv::Vec3b>(h,w)[1] = cloud.points[index].g;
    image.at<cv::Vec3b>(h,w)[2] = cloud.points[index].r;
  }
  return 0;
}
/**
 * @brief This function generates a grabcut mask from pointcloud
 * @param[out] image the image to which the data will be written
 * @param[in] indices the point indices that need to be projected
 * @param[in] cloud the pointcloud from which the RGB data will be taken
 * @return zero if everything went well
 **/
int makeFGMaskFromPointCloud(cv::Mat&                            image,
                             pcl::PointIndices                   indices,
                             pcl::PointCloud<pcl::PointXYZRGB>&  cloud)
{
  assert(image.cols == (int) cloud.width);
  assert(image.rows == (int) cloud.height);

  for(unsigned int i = 0; i < indices.indices.size(); i++)
  {
    int index = indices.indices[i];
    unsigned int h = index/cloud.width;
    unsigned int w = index%cloud.width;
    image.at<char>(h,w) = cv::GC_PR_FGD;
  }
  return 0;
}
/**
 * @brief This function generates a depth image 16 from pointcloud
 * @param[out] image the openCV mat in which the image will be written, must be of type CV_16U
 * @param[in] cloud the pointcloud from which the Z values will be taken an put into shorts
 * @return zero if everything went well
 **/
int makeDepthImage16FromPointCloud(cv::Mat&                            image,
                                   pcl::PointCloud<pcl::PointXYZRGB>&  cloud)
{
  assert(image.cols == (int) cloud.width);
  assert(image.rows == (int) cloud.height);
  assert(image.depth() == CV_16U);

  // First iterate over the pointcloud
  for(unsigned int h = 0; h < cloud.height; h++){
    for(unsigned int w = 0; w < cloud.width; w++){
      float z = cloud.points[h*cloud.width + w].z;
      image.at<short>(h,w) = lrintf(z * 1000);
    }
  }
  return 0;
}
/**
 * @brief This function generates a depth image 16 from pointcloud
 * @param[out] image the openCV mat in which the image will be written, must be of type CV_16U
 * @param[in] cloud the pointcloud from which the Z values will be taken an put into shorts
 * @return zero if everything went well
 **/
int makeDepthImage16FromPointCloud(cv::Mat&                         image,
                                   pcl::PointCloud<pcl::PointXYZ>&  cloud)
{
  assert(image.cols == (int) cloud.width);
  assert(image.rows == (int) cloud.height);
  assert(image.depth() == CV_16U);

  // First iterate over the pointcloud
  for(unsigned int h = 0; h < cloud.height; h++){
    for(unsigned int w = 0; w < cloud.width; w++){
      float z = cloud.points[h*cloud.width + w].z;
      image.at<short>(h,w) = lrintf(z * 1000);
    }
  }
  return 0;
}

} // end namespace LabelSkel
