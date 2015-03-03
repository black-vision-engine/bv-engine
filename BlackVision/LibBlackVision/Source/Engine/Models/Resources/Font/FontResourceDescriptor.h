#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"

#include "System/BasicTypes.h"

#include <string>
#include <vector>

namespace bv
{

#define SUPPROTED_CHARS_FILE L"../dep/Media/fonts/SupportedChars.txt" // FIXME: Sucks as vacuum

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
	SizeType							GetFontSize			() const;
	SizeType							GetBlurSize			() const;
	SizeType							GetOutlineSize		() const;
	bool								GetGenerateMipmaps	() const;
	const std::wstring &				GetAtlasCharSetFile () const;

	static FontResourceDescConstPtr		Create				(	const std::string & fontFileName,
																SizeType fontSize,
																SizeType blurSize,
																SizeType outlineSize,
																bool generateMipmaps,
																const std::wstring & atlasCharSetFile = SUPPROTED_CHARS_FILE );

	explicit							FontResourceDesc	(	const std::string & fontFileName,
																SizeType fontSize,
																SizeType blurSize,
																SizeType outlineSize,
																bool generateMipmaps,
																const std::wstring & atlasCharSetFile );

	static const std::string &			UID();

private:
	std::string		m_fontFileName;
	SizeType		m_fontSize;
	SizeType		m_blurSize;
	SizeType		m_outlineSize;
	bool			m_generateMipmaps;
	std::wstring	m_atlasCharSetFile;

};

} // bv