#include"SelectionController.h"
#include"GraphScene.h"
#include <QColor>

SelectionController::SelectionController(GraphScene* scene): m_scene(scene)
{

}

void SelectionController::setScene(GraphScene* scene)
{
    m_scene = scene;
    clear();

}

void SelectionController::clear()
{
    m_source.clear();
    m_dest.clear();
    applyColors();
}

void SelectionController::handleNodeClicked(const QString& nodeId)
{
    if(m_source.isEmpty())
    {
       m_source = nodeId;// dangzuo 
       m_dest.clear();
       applyColors();
       return;

    }
      if (m_dest.isEmpty())
    {
        if (nodeId == m_source) return; // same no 
        m_dest = nodeId; 
        applyColors();
        return;
    }

      m_source= nodeId;//have both 2 node reset
      m_dest.clear();
      applyColors();
}
void SelectionController::applyColors()
{
  if (!m_scene) return;
  m_scene->resetStyles();
  if (!m_source.isEmpty())//set color
  m_scene->setNodeColor(m_source, QColor(Qt::blue));
  
  if (!m_dest.isEmpty())
  m_scene->setNodeColor(m_dest, QColor(Qt::red));
  


}