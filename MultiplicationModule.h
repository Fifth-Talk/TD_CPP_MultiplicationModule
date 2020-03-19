#include "CHOP_CPlusPlusBase.h"

class MultiplicationModule : public CHOP_CPlusPlusBase
{
public:
	//构造函数与析构函数
	MultiplicationModule(const OP_NodeInfo* info);
	virtual ~MultiplicationModule();

	virtual void		getGeneralInfo(CHOP_GeneralInfo*, const OP_Inputs*, void*) override;
	virtual bool		getOutputInfo(CHOP_OutputInfo*, const OP_Inputs*, void*) override;

	virtual void		execute(CHOP_Output*, const OP_Inputs*, void* reserved) override;

	virtual void		setupParameters(OP_ParameterManager* manager, void *reserved1) override;
	virtual void		getWarningString(OP_String *warning, void *reservedl) override;

private:
	// 警告信息
	char*				warningInfo;
};
