#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

#define SUPPROTED_CHARS_FILE L"Assets/Fonts/SupportedChars.txt" // FIXME: Sucks as vacuum

class FontResourceDesc;
DEFINE_CONST_PTR_TYPE( FontResourceDesc )

class FontResourceDesc : public ResourceDesc,  public std::enable_shared_from_this< ResourceDesc >
{
private:
	static const std::string							uid;
	
protected:
	virtual const std::string &			GetUID				() const override;

public:
	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	const std::string &					GetFontFileName		() const;
	UInt32								GetFontSize			() const;
	UInt32								GetBlurSize			() const;
	UInt32								GetOutlineSize		() const;
	bool								GetGenerateMipmaps	() const;
	const std::wstring &				GetAtlasCharSetFile () const;

	static FontResourceDescConstPtr		Create				(	const std::string & fontFileName,
																UInt32 fontSize,
																UInt32 blurSize,
																UInt32 outlineSize,
																bool generateMipmaps,
																const std::wstring & atlasCharSetFile = SUPPROTED_CHARS_FILE );

	explicit							FontResourceDesc	(	const std::string & fontFileName,
																UInt32 fontSize,
																UInt32 blurSize,
																UInt32 outlineSize,
																bool generateMipmaps,
																const std::wstring & atlasCharSetFile );

	static const std::string &			UID();

private:
	std::string		m_fontFileName;
	UInt32			m_fontSize;
	UInt32			m_blurSize;
	UInt32			m_outlineSize;
	bool			m_generateMipmaps;
	std::wstring	m_atlasCharSetFile;

};

} // bv