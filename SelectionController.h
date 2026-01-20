#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H
#include <QString>

class GraphScene;
//save wheer is star end
class SelectionController
{
public:

SelectionController(GraphScene* scene = nullptr);
//let know whice scence we use
void setScene(GraphScene* scene);

void clear(); 
// sned nodeId
void handleNodeClicked(const QString& nodeId);

bool hasSource() const {return !m_source.isEmpty(); }
bool hasDestination() const {return !m_dest.isEmpty();}
bool ready() const { return hasSource() && hasDestination(); } 
// get id
QString source() const { return m_source; }
QString destination() const {return m_dest;}

private:

void applyColors();

private:
GraphScene* m_scene = nullptr;//huabu ptr
QString m_source;
QString m_dest;

};

#endif