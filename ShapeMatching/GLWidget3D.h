#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderManager.h"
#include <QPen>
#include <QGLWidget>
#include "CGA.h"
#include <QtGui>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "Camera.h"
#include "Stroke.h"
#include "ShapeFeature.h"

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	static enum { MODE_SKETCH = 0, MODE_3DVIEW };

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	RenderManager renderManager;
	bool showWireframe;
	bool showScopeCoordinateSystem;

	cga::CGA cga_system;
	int sketch_step;

	bool ctrlPressed;
	QPoint lastPoint;
	std::vector<Stroke> strokes;
	Stroke* currentStroke;

	std::vector<ShapeFeature> shapeFeatures;

public:
	GLWidget3D(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void drawScene(int drawMode);
	void drawLineTo(const QPoint &endPoint);

	void inferRuleFromSketch();
	glm::vec3 unprojectByPlane(const glm::vec2& point, const glm::vec3& face_point, const glm::vec3& face_normal);
	glm::vec3 unprojectByLine(const glm::vec2& point, const glm::vec3& reference_point, const glm::vec3& vec);
	glm::vec2 normalizeScreenCoordinates(const glm::vec2& point);
	glm::vec3 viewVector(const glm::vec2& point, const glm::mat4& mvMatrix, float focalLength, float aspect);
	bool isStraightLine(Stroke* stroke);
	glm::vec2 findTurningPoint(Stroke* stroke);
	void clearSketch();
	void clear3DModel();
	void analyzeRules();
	void findMatchingRule();

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void initializeGL();
	void paintEvent(QPaintEvent *e);
};

#endif