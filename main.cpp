#include <Windows.h>
#include <sphelper.h>
void main() {
	CoInitialize(NULL);
	HRESULT hr = S_OK;
	const ULONGLONG ullInterest = SPFEI(SPEI_RECOGNITION);
	CComPtr<ISpRecognizer> cpRecoEngine;
	CComPtr<ISpRecoContext> m_cpRecoCtxt;
	CComPtr<ISpRecoGrammar> m_cpDictationGrammar;
	CComPtr<ISpAudio> cpAudio;
	if (SUCCEEDED(hr = cpRecoEngine.CoCreateInstance(CLSID_SpInprocRecognizer))
		&& SUCCEEDED(hr = cpRecoEngine->CreateRecoContext(&m_cpRecoCtxt))
			&& SUCCEEDED(hr = m_cpRecoCtxt->SetInterest(ullInterest, ullInterest))
			&& SUCCEEDED(hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio))
			&& SUCCEEDED(hr = cpRecoEngine->SetInput(cpAudio, TRUE))
			&& SUCCEEDED(hr = cpRecoEngine->SetRecoState(SPRST_ACTIVE))
			&& SUCCEEDED(hr = m_cpRecoCtxt->CreateGrammar(0, &m_cpDictationGrammar))
			&& SUCCEEDED(hr = m_cpDictationGrammar->LoadDictation(NULL, SPLO_STATIC))
			&& SUCCEEDED(hr = m_cpDictationGrammar->SetDictationState(SPRS_ACTIVE))
			&& SUCCEEDED(hr = m_cpRecoCtxt->SetNotifyWin32Event())){
		while (true) {
			hr = m_cpRecoCtxt->WaitForNotifyEvent(INFINITE);
			CSpEvent event;
			while (event.GetFrom(m_cpRecoCtxt) == S_OK){
				if (event.eEventId == SPEI_RECOGNITION){
					CSpDynamicString dstrText;
					if (SUCCEEDED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL))){
						wprintf(L"<Mic Speech Recog>: %s\n", CW2W(dstrText));
					}
				}
			}
		}
	} else {
		m_cpDictationGrammar.Release();
	}
}
