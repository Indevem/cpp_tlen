#include "graphscene.h"
#include "edge.h"
#include <QGraphicsSceneMouseEvent>

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mouseEvent->button() == Qt::RightButton) {
		QList<QGraphicsItem *> elements = items(mouseEvent->scenePos());
		if (elements.count() != 0 && elements.first()->type() == Vertex::Type) {
			Vertex *vertex = qgraphicsitem_cast<Vertex *>(elements.first());
			vertex->removeEdges();
			removeItem(vertex);
			delete vertex;
		}
		else if (elements.count() != 0) {
			Edge *edge = qgraphicsitem_cast<Edge *>(elements.first());
			edge->from()->removeEdge(edge);
			edge->to()->removeEdge(edge);
			removeItem(edge);
			delete edge;
		}
	} //�������� ��������� ������ ������� ����
	else if (mouseEvent->button() == Qt::LeftButton) {
		if (mode_ == InsertVertex) {
			Vertex *vertex = new Vertex();
			vertex->setRect(vertexRect_);
			vertex->setBrush(vertexBrush_);
			vertex->setPen(vertexPen_);
			vertex->setPos(mouseEvent->scenePos());
			addItem(vertex);
			emit(vertexInserted(vertex));
		} //�������� ����� �������
		else if (mode_ == InsertEdge) {
			line_ = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
			line_->setPen(edgePen_);
			line_->setZValue(-100);
			addItem(line_);
		} //������ �������� ������ �����
		QGraphicsScene::mousePressEvent(mouseEvent);
	}
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) {
	if (mode_ == MoveVertex) QGraphicsScene::mouseMoveEvent(mouseEvent);
	else if (mode_ == InsertEdge && line_ != 0) {
		QLineF movedLine(line_->line().p1(), mouseEvent->scenePos()); //��������� ����������� �����
		line_->setLine(movedLine);
	} //���� ������ ��������� ������ �����, ����������� �� ������ ��������������� �����
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	if (mode_ == InsertEdge && line_ != 0) {
		QList<QGraphicsItem *> from = items(line_->line().p1()); //����� ���� ��������� ������
		if (from.count() && from.first() == line_) from.removeFirst();
		QList<QGraphicsItem *> to = items(line_->line().p2());
		if (to.count() && to.first() == line_) to.removeFirst();
		removeItem(line_);
		delete line_;
		if (from.count() && to.count() && from.first() != to.first() &&
			from.first()->type() == Vertex::Type && to.first()->type() == Vertex::Type) {
			Vertex *first = qgraphicsitem_cast<Vertex *>(from.first());
			Vertex *second = qgraphicsitem_cast<Vertex *>(to.first());
			Edge *edge = new Edge(first, second);
			edge->setPen(edgePen_);
			first->addEdge(edge);
			//second->addEdge(edge);
			addItem(edge);
			edge->updatePos();
		} //�������� ������ �����, ���� ������������� ��������� ����������� ��������
		line_ = 0;
	}
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


