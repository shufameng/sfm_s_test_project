#include "sfmchoicequestionitem.h"

//
class SFMChoiceQuestionOptionData {
public:
    SFMChoiceQuestionOptionData() {}
    QString uniqueId;
    QString desc;
};

//
class SFMChoiceQuestionData {
public:
    SFMChoiceQuestionData() {}
    QString sequenceNumber;
    QString desc;
    QList<SFMChoiceQuestionOptionData> optionList;
};

SFMChoiceQuestionItem::SFMChoiceQuestionItem() :
    m_data(new SFMChoiceQuestionData)
{

}
