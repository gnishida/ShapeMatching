#include "GLWidget3D.h"
#include <iostream>
#include "RuleParser.h"
#include "Rectangle.h"
#include "GLUtils.h"
#include "Face.h"
#include "CVUtils.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include "ShapeFeatureLoader.h"

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	setAutoFillBackground(false);

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-4, -5, -8));

	// シャドウマップ用のmodel/view/projection行列を作成
	glm::mat4 light_pMatrix = glm::ortho<float>(-100, 100, -100, 100, 0.1, 200);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 50.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// initialize keys
	ctrlPressed = false;

	// initial sketch step
	sketch_step = cga::STEP_FLOOR;

	// load rules
	cga_system.loadRules();
}

void GLWidget3D::keyPressEvent(QKeyEvent *e) {
	ctrlPressed = false;

	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = true;
		break;
	case Qt::Key_Return:
		cga_system.acceptProposal();
		cga_system.render(&renderManager, true);
		update();
		break;
	case Qt::Key_Space:
		inferRuleFromSketch();
		break;
	default:
		break;
	}
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* e) {
	ctrlPressed = false;
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene(int drawMode) {
	if (drawMode == 0) {
		glUniform1i(glGetUniformLocation(renderManager.program,"depthComputation"), 0);
	} else {
		glUniform1i(glGetUniformLocation(renderManager.program,"depthComputation"), 1);
	}

	glUniform1i(glGetUniformLocation(renderManager.program,"lineRendering"), 0);

	if (showScopeCoordinateSystem) {
		renderManager.renderAll(showWireframe);
	} else {
		renderManager.renderAllExcept("axis", showWireframe);
	}
}

void GLWidget3D::drawLineTo(const QPoint &endPoint) {
	currentStroke->points.push_back(glm::vec2(endPoint.x(), endPoint.y()));

	lastPoint = endPoint;
}

void GLWidget3D::inferRuleFromSketch() {
	int face_index;
	std::vector<glm::vec3> face_points;

	// ワールド座標系でのカメラ座標
	glm::vec3 cameraPos = camera.cameraPosInWorld();

	// 視線ベクトル
	glm::vec3 view_v1 = viewVector(strokes[0].points[0], camera.mvMatrix, camera.f(), camera.aspect());

	// faceを取得
	cga::Face face;
	if (cga_system.hitFace(cameraPos, view_v1, face)) {
		// strokeの3d座標を計算
		for (int i = 0; i < strokes.size(); ++i) {
			if (strokes[i].points3d.size() == strokes[i].points.size()) continue;

			for (int k = 0; k < strokes[i].points.size(); ++k) {
				strokes[i].points3d.push_back(unprojectByPlane(strokes[i].points[k], face.points[0], face.normals[0]));
			}		
		}

		// strokeから、ルールを探す
		face.shape->findRule(strokes, sketch_step, &cga_system);

		// CGAモデルを生成しなおす
		cga_system.generateProposal();

		// 描画する
		cga_system.render(&renderManager, true);
	}

	strokes.clear();

	update();
}

glm::vec3 GLWidget3D::unprojectByPlane(const glm::vec2& point, const glm::vec3& face_point, const glm::vec3& face_normal) {
	glm::vec3 cameraPos = camera.cameraPosInWorld();
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * camera.aspect(), (height() * 0.5f - point.y) * 2.0f / height(), -camera.f());
	dir = glm::vec3(glm::inverse(camera.mvMatrix) * glm::vec4(dir, 0));

	glm::vec3 intPt = glutils::rayPlaneIntersection(cameraPos, dir, face_point, face_normal);
	return intPt;
}

glm::vec3 GLWidget3D::unprojectByLine(const glm::vec2& point, const glm::vec3& reference_point, const glm::vec3& vec) {
	glm::vec3 cameraPos = camera.cameraPosInWorld();
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * camera.aspect(), (height() * 0.5f - point.y) * 2.0f / height(), -camera.f());
	dir = glm::vec3(glm::inverse(camera.mvMatrix) * glm::vec4(dir, 0));

	glm::vec3 intPt = glutils::lineLineIntersection(cameraPos, dir, reference_point, vec, 0, 1);
	return intPt;
}

glm::vec2 GLWidget3D::normalizeScreenCoordinates(const glm::vec2& point) {
	return glm::vec2((point.x - width() * 0.5f) * 2.0f / width(), (height() * 0.5f - point.y) * 2.0f / height());
}

glm::vec3 GLWidget3D::viewVector(const glm::vec2& point, const glm::mat4& mvMatrix, float focalLength, float aspect) {
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * aspect, (height() * 0.5f - point.y) * 2.0f / height(), -focalLength);
	return glm::vec3(glm::inverse(mvMatrix) * glm::vec4(dir, 0));
}

bool GLWidget3D::isStraightLine(Stroke* stroke) {
	glm::vec2 midPt = stroke->points[stroke->points.size() * 0.5];
	
	glm::vec2 vec1 = glm::normalize(stroke->points.back() - stroke->points[0]);
	glm::vec2 vec2 = glm::normalize(midPt - stroke->points[0]);
	if (glm::dot(vec1, vec2) > 0.95f) {
		std::cout << "Straight line" << std::endl;
		std::cout << "[0]: " << stroke->points[0].x << "," << stroke->points[0].y << std::endl;
		std::cout << "[mid]: " << midPt.x << "," << midPt.y << std::endl;
		std::cout << "[back]: " << stroke->points.back().x << "," << stroke->points.back().y << std::endl;
		std::cout << "vec1: " << vec1.x << "," << vec1.y << std::endl;
		std::cout << "vec2: " << vec2.x << "," << vec2.y << std::endl;
		return true;
	} else {
		std::cout << "Not Straight line" << std::endl;
		return false;
	}
}

glm::vec2 GLWidget3D::findTurningPoint(Stroke* stroke) {
	int index1 = stroke->points.size() * 0.2;
	int index2 = stroke->points.size() * 0.8;
	
	glm::vec2 v = glm::normalize(stroke->points.back() - stroke->points[0]);

	float min_dot = 1.0f;
	int min_index = -1;

	for (int i = index1; i < index2; ++i) {
		float dot = fabs(glm::dot(glm::normalize(stroke->points[i] - stroke->points[0]), v));
		if (dot < min_dot) {
			min_dot = dot;
			min_index = i;
		}
	}

	return stroke->points[min_index];
}

void GLWidget3D::clearSketch() {
	strokes.clear();
	if (currentStroke != NULL) {
		delete currentStroke;
		currentStroke = NULL;
	}
}

void GLWidget3D::clear3DModel() {
	renderManager.removeObjects();

	std::vector<Vertex> vertices;
	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(0, -1, 0));
	mat = glm::rotate(mat, (float)(-M_PI * 0.5f), glm::vec3(1, 0, 0));
	glutils::drawGrid(100, 100, 2, glm::vec4(0.7, 0.7, 0.7, 1), glm::vec4(1, 1, 1, 1), mat, vertices);

	renderManager.addObject("grid", "", vertices);
}

void GLWidget3D::analyzeRules() {
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(renderManager.program);

	glDisable(GL_TEXTURE_2D);

	// pass the light direction to the shader
	//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
	glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	
	glUniform1i(glGetUniformLocation(renderManager.program,"depthComputation"), 0);
	glUniform1i(glGetUniformLocation(renderManager.program,"lineRendering"), 1);

	shapeFeatures.clear();
	char filenames[][255] = {"../cga/simpleMass.xml", "../cga/LshapeMass.xml"};
	int num_samples[] = {1, 25};

	QFile xmlFile("features.xml");
	xmlFile.open(QIODevice::WriteOnly);
	QXmlStreamWriter xmlWriter(&xmlFile);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("shapeFeatures");

	for (int fi = 0; fi < 2; ++fi) {
		try {
			cga::parseRules(filenames[fi], cga_system.ruleSet);
		} catch (const char* ex) {
			std::cout << "ERROR:" << std::endl << ex << std::endl;
		}

		for (int si = 0; si < num_samples[fi]; ++si) {
			std::cout << si << std::endl;
			// select random values for attrs
			for (auto it = cga_system.ruleSet.attrs.begin(); it != cga_system.ruleSet.attrs.end(); ++it) {
				if (it->first == "bldg_height") {
					it->second = boost::lexical_cast<std::string>(40);
				} else if (it->first == "bldg_height_lower_ratio") {
					it->second = boost::lexical_cast<std::string>((int)(si / 5) * 0.1f + 0.3f);
				} else {
					it->second = boost::lexical_cast<std::string>((int)(si % 5) * 0.1f + 0.3f);
				}
			}

			renderManager.removeObjects();
			try {
				cga_system.generate();
				cga_system.render(&renderManager, true);
			} catch (const char* ex) {
				std::cout << "ERROR:" << std::endl << ex << std::endl;
			}


			camera.pos.z = 100.0f;
			int num_pitch = 10;
			for (int pi = 0; pi < num_pitch; ++pi) {
				int pitch = pi * 90.0f / (float)num_pitch;
				camera.xrot = pitch;

				int num_yaw = 36;
				for (int yi = 0; yi < num_yaw; ++yi) {
					int yaw = yi * 360.0f / (float)num_yaw;

					camera.yrot = yaw;
					camera.updateMVPMatrix();

					glClearColor(1, 1, 1, 0);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glEnable(GL_DEPTH_TEST);

					// Model view projection行列をシェーダに渡す
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
					glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

					renderManager.render("shape", true);


					unsigned char* data = new unsigned char[sizeof(unsigned char) * 3 * width() * height()];
					glReadPixels(0, 0, width(), height(), GL_BGR, GL_UNSIGNED_BYTE, data);
					cv::Mat img(height(), width(), CV_8UC3, data);
					cv::flip(img, img, 0);
					cv::cvtColor(img, img, CV_BGR2GRAY);
					//cv::imwrite("test.jpg", img);
					cv::GaussianBlur(img, img, cv::Size(11, 11), 11, 11);
					//cv::imwrite("test2.jpg", img);
					cv::threshold(img, img, 253, 255, 0);
					//cv::imwrite("test3.jpg", img);

					cv::Rect roi = cvutils::computeBoundingBoxFromImage(img);
					cv::Mat img2;
					img(roi).copyTo(img2);
					//cv::imwrite("test4.jpg", img2);

					cv::resize(img2, img2, cv::Size(img2.cols * 0.2, img2.rows * 0.2));
					//cv::imwrite("test5.jpg", img2);

					char fn[256];
					sprintf(fn, "features/%d_%d_%d_%d.jpg", fi, si, pitch, yaw);
					cv::imwrite(fn, img2);
				
					delete [] data;


					xmlWriter.writeStartElement("shapeFeature");
					xmlWriter.writeStartElement("camera");
					xmlWriter.writeAttribute("pitch_angle", QString::number(pitch));
					xmlWriter.writeAttribute("yaw_angle", QString::number(yaw));
					xmlWriter.writeEndElement();
					xmlWriter.writeStartElement("image");
					xmlWriter.writeAttribute("path", fn);
					xmlWriter.writeEndElement();
					xmlWriter.writeStartElement("cga");
					xmlWriter.writeAttribute("path", filenames[fi]);
					xmlWriter.writeEndElement();
					for (auto it = cga_system.ruleSet.attrs.begin(); it != cga_system.ruleSet.attrs.end(); ++it) {
						xmlWriter.writeStartElement("attr");
						xmlWriter.writeAttribute("name", it->first.c_str());
						xmlWriter.writeAttribute("value", it->second.c_str());
						xmlWriter.writeEndElement();
					}
					xmlWriter.writeEndElement();


					shapeFeatures.push_back(ShapeFeature(pitch, yaw, img2, filenames[fi], cga_system.ruleSet.attrs));
				}
			}
		}
	}
	xmlWriter.writeEndDocument();
	xmlFile.close();

	// OpenGLの設定を元に戻す
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void GLWidget3D::findMatchingRule() {
	/*
	shapeFeatures.clear();
	
	char filenames[][255] = {"simpleMass.xml", "LshapeMass.xml"};

	// load features from files
	QDir dir("features");
	QStringList filters;
	filters << "*.jpg";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		QStringList list = fileInfoList[i].baseName().split("_");
		int fi = list[0].toInt();
		int si = list[1].toInt();
		int pitch_angle = list[2].toInt();
		int yaw_angle = list[3].toInt();
		cv::Mat img = cv::imread(fileInfoList[i].absoluteFilePath().toUtf8().constData(), CV_LOAD_IMAGE_GRAYSCALE);

		shapeFeatures.resize(fi + 1);

		std::map<std::string, std::string> attrs;
		if (fi == 0) {
			attrs["bldg_height"] = boost::lexical_cast<std::string>(40);
		} else {
			attrs["bldg_height"] = boost::lexical_cast<std::string>(40);
			attrs["bldg_height_lower_ratio"] = boost::lexical_cast<std::string>((int)(si / 5) * 0.1 + 0.3);
			attrs["bldg_upper_width_ratio"] = boost::lexical_cast<std::string>((int)(si % 5) * 0.1 + 0.3);
		}


		shapeFeatures[fi].push_back(ShapeFeature(pitch_angle, yaw_angle, img, "../cga/" + std::string(filenames[fi]), attrs));

	}

	std::cout << "load done." << std::endl;
	*/

	// create QImage object of the sketch
	QImage sketch(QSize(width(), height()), QImage::Format_RGB888);
	sketch.fill(QColor(255, 255, 255));

	QPainter painter;
	painter.begin(&sketch);
	painter.setPen(QPen(QColor(0, 0, 0)));
	for (int i = 0; i < strokes.size(); ++i) {
		for (int k = 0; k < strokes[i].points.size() - 1; ++k) {
			painter.drawLine(strokes[i].points[k].x, strokes[i].points[k].y, strokes[i].points[k+1].x, strokes[i].points[k+1].y);
		}
	}
	painter.end();

	//sketch.save("tet.jpg");

	cv::Mat matSketch(sketch.height(), sketch.width(), CV_8UC3, const_cast<uchar*>(sketch.bits()), sketch.bytesPerLine());

	// convert the sketch to grayscale
	cv::cvtColor(matSketch, matSketch, CV_BGR2GRAY);

	// blur the sketch
	cv::GaussianBlur(matSketch, matSketch, cv::Size(11, 11), 11, 11);

	// conver the sketch to mono color
	cv::threshold(matSketch, matSketch, 253, 255, 0);

	// rectangle
	cv::Rect roi = cvutils::computeBoundingBoxFromImage(matSketch);
	cv::Mat matSketch2;
	matSketch(roi).copyTo(matSketch2);

	cv::resize(matSketch2, matSketch2, cv::Size(matSketch2.cols * 0.2, matSketch2.rows * 0.2));

	cv::imwrite("test_sketch.jpg", matSketch2);


	//
	// compare the sketch with the shape features extracted from the CGA grammars
	float min_diff = std::numeric_limits<float>::max();
	ShapeFeature min_sf;

	for (int i = 0; i < shapeFeatures.size(); ++i) {
		cv::Mat shapeMat2;
		cv::resize(shapeFeatures[i].image, shapeMat2, matSketch2.size());

		cv::Mat matDiff;
		cv::absdiff(shapeMat2, matSketch2, matDiff);
		float diff = cvutils::mat_sum(matDiff);// / cvutils::mat_sum(shapeMat2);

		if (diff < min_diff) {
			min_diff = diff;
			min_sf = shapeFeatures[i];
		}
	}

	cv::imwrite("test_matched.jpg", min_sf.image);



	renderManager.removeObjects();

	std::vector<Vertex> vertices;
	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(0, -1, 0));
	mat = glm::rotate(mat, (float)(-M_PI * 0.5f), glm::vec3(1, 0, 0));
	glutils::drawGrid(100, 100, 2, glm::vec4(0.7, 0.7, 0.7, 1), glm::vec4(1, 1, 1, 1), mat, vertices);

	renderManager.addObject("grid", "", vertices);


	try {
		cga::parseRules(min_sf.cga_filename, cga_system.ruleSet);

		// set parameter values
		for (auto it = min_sf.attrs.begin(); it != min_sf.attrs.end(); ++it) {
			cga_system.ruleSet.attrs[it->first] = it->second;
		}

		cga_system.generate();
		cga_system.render(&renderManager, true);
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}

	camera.yrot = min_sf.yaw_angle;
	camera.xrot = min_sf.pitch_angle;
	camera.pos.z = 100;
	camera.updateMVPMatrix();

	update();
}

void GLWidget3D::resizeGL(int width, int height) {
	// OpenGLの設定を更新
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);
}

void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	if (ctrlPressed) { // move camera
		camera.mousePress(e->x(), e->y());
	} else { // sketch
		lastPoint = e->pos();
		currentStroke = new Stroke(glm::vec2(e->x(), e->y()));
	}

	update();
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
	if (currentStroke != NULL) {
		if (currentStroke->points.size() > 3 && currentStroke->length() > 10) {
			strokes.push_back(*currentStroke);
		}

		delete currentStroke;
		currentStroke = NULL;
	}

	update();
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (ctrlPressed) {
		if (e->buttons() & Qt::LeftButton) { // Rotate
			camera.rotate(e->x(), e->y());
		} else if (e->buttons() & Qt::MidButton) { // Move
			camera.move(e->x(), e->y());
		} else if (e->buttons() & Qt::RightButton) { // Zoom
			camera.zoom(e->x(), e->y());
		}
		strokes.clear();
	} else { // sketch
		drawLineTo(e->pos());
	}

	update();
}

void GLWidget3D::initializeGL() {
	renderManager.init("../shaders/vertex.glsl", "../shaders/geometry.glsl", "../shaders/fragment.glsl", 8192);
	showWireframe = true;
	showScopeCoordinateSystem = false;

	std::vector<Vertex> vertices;
	glm::mat4 mat = glm::translate(glm::mat4(), glm::vec3(0, -1, 0));
	mat = glm::rotate(mat, (float)(-M_PI * 0.5f), glm::vec3(1, 0, 0));
	glutils::drawGrid(100, 100, 2, glm::vec4(0.7, 0.7, 0.7, 1), glm::vec4(1, 1, 1, 1), mat, vertices);

	renderManager.addObject("grid", "", vertices);


	// CGA initial mass
	cga::Rectangle* lot = new cga::Rectangle("Lot", glm::translate(glm::rotate(glm::mat4(), (float)(-M_PI * 0.5f), glm::vec3(1, 0, 0)), glm::vec3(-17.5, -12.5, 0)), glm::mat4(), 35, 25, glm::vec3(1, 1, 1));
	cga_system.axiom = boost::shared_ptr<cga::Shape>(lot);

	/*
	try {
		cga::parseRules("../cga/simpleMass.xml", cga_system.ruleSet);
		cga_system.generate();
		cga_system.render(&renderManager, true);
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}
	*/

	loadShapeFeatures("features.xml", shapeFeatures);

	currentStroke = NULL;
}

void GLWidget3D::paintEvent(QPaintEvent *event) {
	// OpenGLで描画
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(renderManager.program);

	glClearColor(1, 1, 1, 0);
	//glClearColor(0.443, 0.439, 0.458, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// for transparacy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Model view projection行列をシェーダに渡す
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

	// pass the light direction to the shader
	//glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
	glUniform3f(glGetUniformLocation(renderManager.program, "lightDir"), light_dir.x, light_dir.y, light_dir.z);
	
	drawScene(0);

	// OpenGLの設定を元に戻す
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// QPainterで描画
	QPainter painter(this);
	painter.setPen(QPen(QColor(0, 0, 0)));
	painter.setOpacity(0.5);
	for (int i = 0; i < strokes.size(); ++i) {
		for (int k = 0; k < strokes[i].points.size() - 1; ++k) {
			painter.drawLine(strokes[i].points[k].x, strokes[i].points[k].y, strokes[i].points[k+1].x, strokes[i].points[k+1].y);
		}
	}
	if (currentStroke != NULL) {
		for (int k = 0; k < currentStroke->points.size() - 1; ++k) {
			painter.drawLine(currentStroke->points[k].x, currentStroke->points[k].y, currentStroke->points[k+1].x, currentStroke->points[k+1].y);
		}
	}
	painter.end();

	glEnable(GL_DEPTH_TEST);
}
