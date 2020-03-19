#include "MultiplicationModule.h"

extern "C"
{

	DLLEXPORT
	void FillCHOPPluginInfo(CHOP_PluginInfo *info)
	{
		// ���ɸ���
		info->apiVersion = CHOPCPlusPlusAPIVersion;

		// �����Զ���ڵ�ʱ��Ҫ���õĲ����������ݲ�����
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
		// ����CHOPʵ������������CHOP�����DLLʱ����
		// ����Ϊ�����Զ�����������
		return new MultiplicationModule(info);
	}

	DLLEXPORT
	void DestroyCHOPInstance(CHOP_CPlusPlusBase* instance)
	{
		// ɾ��CHOPʵ���������ر�Touchdesigner��ɾ��CHOP�����DLLʱ����
		// ����Ϊ�����Զ�����������
		delete (MultiplicationModule*)instance;
	}

};

// ��ʼ���Զ������
MultiplicationModule::MultiplicationModule(const OP_NodeInfo* info)
{
	// ��ʼ��������Ϣ
	warningInfo = NULL;
}

// DLL�˳�ʱִ��
MultiplicationModule::~MultiplicationModule()
{

}

void MultiplicationModule::getGeneralInfo(CHOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void* reserved1)
{
	// ���ƥ���һ�������CHOP
	ginfo->inputMatchIndex = 0;
}

bool MultiplicationModule::getOutputInfo(CHOP_OutputInfo* info, const OP_Inputs* inputs, void* reservedl)
{
	return false;
}

void MultiplicationModule::execute(CHOP_Output* output, const OP_Inputs* inputs, void* reservedl)
{
	// ��ȡ�Զ������
	double parValue = inputs->getParDouble("Multiple");
	

	if (inputs->getNumInputs() == 0) {
		// ���þ�����Ϣ
		warningInfo = "Has not Inputs CHOP.";
		return;
	}

	// ���������Ϣ
	warningInfo = NULL;

	/*
		δ����getOutputInfo()��������£�DLLĬ�������һ�������CHOP������
		----------
		output->numChannels()		��ȡ���ͨ����
		output->numSamples()		��ȡ���������
		output->channels[i][j]		��������ĵ� i ͨ���� j ������

		inputs->getInputCHOP(0)->getChannelData(i)[j]		��ȡ��һ�������CHOP�� i ͨ���� j ������
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
		
		// ��������
		np.name = "Multiple";
		// ������ʾ��ǩ
		np.label = "multiple";
		// ����Ĭ��ֵ
		np.defaultValues[0] = 2.0;
		// ��������С/���ֵ��������һ����ֵ�Ĳ������ã�
		np.minSliders[0] = -10;
		np.maxSliders[0] = 10;
		// ��������С/���ֵ����clampMins/clampMaxes��������Ϊtrue���ã�
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
