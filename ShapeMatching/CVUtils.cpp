#include "CVUtils.h"

namespace cvutils {

/**
 * 行列の指定した行、列の値を返却する。
 *
 * @param m		行列
 * @param r		行
 * @param c		列
 * @return		値
 */
double mat_get_value(const cv::Mat& m, int r, int c) {
	if (m.type() == CV_8U) {
		return m.at<uchar>(r, c);
	} else if (m.type() == CV_8UC3) {
		return m.at<cv::Vec3i>(r, c)[0];
	} else if (m.type() == CV_32F) {
		return m.at<float>(r, c);
	} else if (m.type() == CV_64F) {
		return m.at<double>(r, c);
	} else {
		return 0.0;
	}
}

/**
 * 行列の指定した行、列の値を更新する。
 *
 * @param m		行列
 * @param r		行
 * @param c		列
 * @return		値
 */
void mat_set_value(cv::Mat& m, int r, int c, double val) {
	if (m.type() == CV_8U) {
		m.at<uchar>(r, c) = val;
	} else if (m.type() == CV_8UC3) {
		m.at<cv::Vec3i>(r, c)[0] = val;
		m.at<cv::Vec3i>(r, c)[1] = val;
		m.at<cv::Vec3i>(r, c)[2] = val;
	} else if (m.type() == CV_32F) {
		m.at<float>(r, c) = val;
	} else if (m.type() == CV_64F) {
		m.at<double>(r, c) = val;
	}
}

/**
 * 行列の要素和を返却する。
 *
 * @param m		行列
 */
double mat_sum(const cv::Mat& m) {
	return cv::sum(m)[0];
}

/**
 * 背景色が白色、線の色が黒色の時に、線を囲むbounding boxを計算する。
 */
cv::Rect computeBoundingBoxFromImage(const cv::Mat& img) {
	cv::Rect rect;

	cv::Mat row_img, col_img;
	cv::reduce(img, row_img, 0, CV_REDUCE_MIN);
	cv::reduce(img, col_img, 1, CV_REDUCE_MIN);

	{ // horizontal scan
		bool flag = false;
		for (int c = 0; c < row_img.cols; ++c) {
			double val = mat_get_value(row_img, 0, c);

			if (!flag) {
				if (val == 0) {
					rect.x = c;
					flag = true;
				}
			} else {
				if (val > 0) {
					rect.width = c - rect.x + 1;
					break;
				}
			}
		}
	}

	{ // vertical scan
		bool flag = false;
		for (int r = 0; r < col_img.rows; ++r) {
			double val = mat_get_value(col_img, r, 0);

			if (!flag) {
				if (val == 0) {
					rect.y = r;
					flag = true;
				}
			} else {
				if (val > 0) {
					rect.height = r - rect.y + 1;
					break;
				}
			}
		}
	}

	return rect;
}

}