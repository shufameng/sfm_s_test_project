#ifndef SFMCHOICEQUESTIONITEM_H
#define SFMCHOICEQUESTIONITEM_H

#include <QGraphicsRectItem>

class SFMChoiceQuestionData;
class SFMChoiceQuestionItem
{
public:
    SFMChoiceQuestionItem();

protected:
    SFMChoiceQuestionData *m_data;
};

#endif // SFMCHOICEQUESTIONITEM_H
