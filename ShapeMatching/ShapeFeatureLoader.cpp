#include "ShapeFeatureLoader.h"
#include <QFile>
#include <QDomDocument>

void loadShapeFeatures(const std::string& filename, std::vector<ShapeFeature>& features) {
	features.clear();

	QFile file(filename.c_str());
	if (!file.exists()) return;

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode feature_node = root.firstChild();
	while (!feature_node.isNull()) {
		if (feature_node.toElement().tagName() == "shapeFeature") {
			float pitch_angle;
			float yaw_angle;
			cv::Mat img;
			std::string filename;
			std::map<std::string, std::string> attrs;

			QDomNode child_node = feature_node.firstChild();
			while (!child_node.isNull()) {
				if (child_node.toElement().tagName() == "camera") {
					pitch_angle = child_node.toElement().attribute("pitch_angle").toFloat();
					yaw_angle = child_node.toElement().attribute("yaw_angle").toFloat();
				} else if (child_node.toElement().tagName() == "image") {
					QString imageFile = child_node.toElement().attribute("path");
					img = cv::imread(imageFile.toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE);
				} else if (child_node.toElement().tagName() == "cga") {
					filename = child_node.toElement().attribute("path").toUtf8().constData();
				} else if (child_node.toElement().tagName() == "attr") {
					std::string attr_name = child_node.toElement().attribute("name").toUtf8().constData();
					std::string attr_value = child_node.toElement().attribute("value").toUtf8().constData();
					attrs[attr_name] = attr_value;
				}

				child_node = child_node.nextSibling();
			}

			features.push_back(ShapeFeature(pitch_angle, yaw_angle, img, filename, attrs));
		}

		feature_node = feature_node.nextSibling();
	}
}
