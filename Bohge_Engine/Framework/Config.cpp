#include "Config.h"





namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------------------
	Config::Config(void)
	{
		m_RootPath.reserve( IO_MAX_LITE_PATH_SIZE );
		m_ResourcePath.reserve( IO_MAX_LITE_PATH_SIZE );
		m_LitePath.reserve( IO_MAX_LITE_PATH_SIZE );
	}
	//---------------------------------------------------------------------------------------------------------
	Config::~Config(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void Config::SetViewSize( const vector4d& size )
	{
		m_ViewSize = size;
	}
	//---------------------------------------------------------------------------------------------------------
	void Config::SetResolution( const vector2d& res )
	{
		m_Resolution = res;
	}
	//---------------------------------------------------------------------------------------------------------
	void Config::SetRootPath( const eastl::string& path )
	{
		m_RootPath = path;
	}
	//---------------------------------------------------------------------------------------------------------
	void Config::SetResourcePath( const eastl::string& path )
	{
		m_ResourcePath = path;
	}
	//---------------------------------------------------------------------------------------------------------
	const vector4d& Config::GetViewSize() const
	{
		return m_ViewSize;
	}
	//---------------------------------------------------------------------------------------------------------
	const vector2d& Config::GetResolution() const
	{
		return m_Resolution;
	}
	//---------------------------------------------------------------------------------------------------------
	const eastl::string& Config::GetRootPath() const
	{
		return m_RootPath;
	}
	//---------------------------------------------------------------------------------------------------------
	const eastl::string& Config::GetResourcePath() const
	{
		return m_ResourcePath;
	}
	//---------------------------------------------------------------------------------------------------------
	void Config::SetLitePath( const eastl::string& path )
	{
		m_LitePath = path;
	}
	//---------------------------------------------------------------------------------------------------------
	const eastl::string& Config::GetLitePath() const
	{
		return m_LitePath;
	}
}