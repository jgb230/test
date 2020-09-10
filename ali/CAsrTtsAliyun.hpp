/*
 * CAsrTtsAliyun.hpp
 *
 *  Created on: 2020年5月11日
 *      Author: tianhuajian
 */

#ifndef CASRTTSALIYUN_HPP_
#define CASRTTSALIYUN_HPP_

#include <string>
#include <mutex>
#include "CAsrTtsBase.hpp"

struct token_info
{
		long m_expireTime;
		std::string m_token;
};
class CAsrTtsAliyun: public CAsrTtsBase
{
	public:
		CAsrTtsAliyun();
		~CAsrTtsAliyun();
		bool asrRequest(const asr_parameter *asr_param,const char* audioFile,char*result);
		bool ttsRequest(const tts_parameter *tts_param,const char* contents,
				const char* audioFile);
	private:
		bool generateToken( std::string akId,  std::string akSecret,std::string&token,std::string url);
		bool generateTokenHttp(const std::string url, const std::string &akId, const std::string &akSecret,std::string&token);
		int text_to_speech(const char* src_text, const char* des_path, const tts_parameter *tts_param);
		int speechSynthesizerFile(const char* src_text, const char* des_path, const tts_parameter *tts_param);
		std::string getISO8601Time(time_t tm = 0);
		std::string getUniqueNonce();
		std::string createStringToSign(std::string &method, std::string &urlPath, std::string &queryString);
		std::string sign(std::string &stringToSign, std::string &accessKeySecret);
	private:
		std::map<std::string, token_info> m_token_map;
};

#endif /* CASRTTSALIYUN_HPP_ */
