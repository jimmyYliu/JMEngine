#pragma once

#include <spdlog/spdlog.h>

namespace JMEngine
{
	// 单例类，建议只使用宏定义功能
	class LogSystem final
	{
	private:
		LogSystem();
		~LogSystem();
		LogSystem(const LogSystem& logSystem) = delete;
		LogSystem& operator=(const LogSystem& logSystem) = delete;
		static std::shared_ptr<spdlog::logger> m_logger;

	public:
		[[nodiscard]] inline static std::shared_ptr<spdlog::logger>& GetLogger()
		{
			static LogSystem g_logger_system;
			return m_logger;
		}
	};

#define LOG_DEBUG(...)		JMEngine::LogSystem::GetLogger()->debug("[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__)
#define LOG_INFO(...)		JMEngine::LogSystem::GetLogger()->info("[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__)
#define LOG_WARN(...)		JMEngine::LogSystem::GetLogger()->warn("[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__)
#define LOG_ERROR(...)		JMEngine::LogSystem::GetLogger()->error("[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__)
#define LOG_FATAL(...)		JMEngine::LogSystem::GetLogger()->critical("[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__);throw std::runtime_error(__VA_ARGS__)

}