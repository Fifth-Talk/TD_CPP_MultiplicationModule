#include "MultiplicationModule.h"

extern "C"
{

	DLLEXPORT
	void FillCHOPPluginInfo(CHOP_PluginInfo *info)
	{
		// 不可更改
		info->apiVersion = CHOPCPlusPlusAPIVersion;

		// 制作自定义节点时需要设置的参数，本例暂不讨论
		info->customOPInfo.opType->setString("Customsignal");
		info->customOPInfo.opLabel->setString("Custom Signal");
		info->customOPInfo.authorName->setString("Author Name");
		info->customOPInfo.authorEmail->setString("email@email.com");
		info->customOPInfo.minInputs = 0;
		info->customOPInfo.maxInputs = 1;
	}

	DLLEXPORT
	CHOP_CPlusPlusBase* CreateCHOPInstance(const OP_NodeInfo* info)
	{
		// 创建CHOP实例函数，创建CHOP或加载DLL时调用
		// 设置为我们自定义的类的名称
		return new MultiplicationModule(info);
	}

	DLLEXPORT
	void DestroyCHOPInstance(CHOP_CPlusPlusBase* instance)
	{
		// 删除CHOP实例函数，关闭Touchdesigner，删除CHOP或更改DLL时调用
		// 设置为我们自定义的类的名称
		delete (MultiplicationModule*)instance;
	}

};

// 初始化自定义参数
MultiplicationModule::MultiplicationModule(const OP_NodeInfo* info)
{
	// 初始化警告信息
	warningInfo = NULL;
}

// DLL退出时执行
MultiplicationModule::~MultiplicationModule()
{

}

void MultiplicationModule::getGeneralInfo(CHOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void* reserved1)
{
	// 输出匹配第一个输出的CHOP
	ginfo->inputMatchIndex = 0;
}

bool MultiplicationModule::getOutputInfo(CHOP_OutputInfo* info, const OP_Inputs* inputs, void* reservedl)
{
	return false;
}

void MultiplicationModule::execute(CHOP_Output* output, const OP_Inputs* inputs, void* reservedl)
{
	// 获取自定义参数
	double parValue = inputs->getParDouble("Multiple");
	

	if (inputs->getNumInputs() == 0) {
		// 设置警告信息
		warningInfo = "Has not Inputs CHOP.";
		return;
	}

	// 清除警告信息
	warningInfo = NULL;

	/*
		未设置getOutputInfo()函数情况下，DLL默认输出第一个输入的CHOP的数据
		----------
		output->numChannels()		获取输出通道数
		output->numSamples()		获取输出样本数
		output->channels[i][j]		设置输出的第 i 通道第 j 个数据

		inputs->getInputCHOP(0)->getChannelData(i)[j]		获取第一个输入的CHOP第 i 通道第 j 个数据
	*/

	for (int i = 0; i < output->numChannels; i++) {
		for (int j = 0; j < output->numSamples; j++) {
			output->channels[i][j] = float(inputs->getInputCHOP(0)->getChannelData(i)[j] * parValue);
		}
	}

	return;
}

void MultiplicationModule::setupParameters(OP_ParameterManager*manager, void* reservedl)
{
	{
		OP_NumericParameter np;
		
		// 参数名称
		np.name = "Multiple";
		// 参数显示标签
		np.label = "multiple";
		// 参数默认值
		np.defaultValues[0] = 2.0;
		// 滑动条最小/最大值（仅包含一个数值的参数适用）
		np.minSliders[0] = -10;
		np.maxSliders[0] = 10;
		// 参数的最小/最大值（仅clampMins/clampMaxes参数设置为true适用）
		np.minValues[0] = -100;
		np.maxValues[0] = 100;
		np.clampMins[0] = true;
		np.clampMaxes[0] = true;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
}

void MultiplicationModule::getWarningString(OP_String * warning, void * reservedl)
{
	warning->setString(warningInfo);
}
