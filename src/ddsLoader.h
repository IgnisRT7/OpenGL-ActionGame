/**
*	@file ddsLoader.h
*/

#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <GL/glew.h>

namespace DDSLoader{

	/**
	* FOURCC���쐬����.
	*/
	#define MAKE_FOURCC(a, b, c, d) \
	  static_cast<uint32_t>(a + (b << 8) + (c << 16) + (d << 24))

	/**
	* DDS�摜���.
	*/
	struct DDSPixelFormat
	{
		uint32_t size; ///< ���̍\���̂̃o�C�g��(32).
		uint32_t flgas; ///< �摜�Ɋ܂܂��f�[�^�̎�ނ������t���O.
		uint32_t fourCC; ///< �摜�t�H�[�}�b�g������FOURCC.
		uint32_t rgbBitCount; ///< 1�s�N�Z���̃r�b�g��.
		uint32_t redBitMask; ///< �ԗv�f���g�������������r�b�g.
		uint32_t greenBitMask; ///< �Ηv�f���g�������������r�b�g.
		uint32_t blueBitMask; ///< �v�f���g�������������r�b�g.
		uint32_t alphaBitMask; ///< �����v�f���g�������������r�b�g.
	};

	/**
	* DDS�t�@�C���w�b�_.
	*/
	struct DDSHeader
	{
		uint32_t size;  ///< ���̍\���̂̃o�C�g��(124).
		uint32_t flags; ///< �ǂ̃p�����[�^���L�����������t���O.
		uint32_t height; ///< �摜�̍���(�s�N�Z����).
		uint32_t width; ///< �摜�̕�(�s�N�Z����).
		uint32_t pitchOrLinearSize; ///< ���̃o�C�g���܂��͉摜1���̃o�C�g��.
		uint32_t depth; ///< �摜�̉��s��(����)(3�����e�N�X�`�����Ŏg�p).
		uint32_t mipMapCount; ///< �܂܂�Ă���~�b�v�}�b�v���x����.
		uint32_t reserved1[11]; ///< (�����̂��߂ɗ\�񂳂�Ă���).
		DDSPixelFormat ddspf; ///< DDS�摜���.
		uint32_t caps[4]; ///< �܂܂�Ă���摜�̎��.
		uint32_t reserved2; ///< (�����̂��߂ɗ\�񂳂�Ă���).
	};

	/**
	*	�o�C�g�񂩂琔�l�𕜌�����
	*
	*	@param p		�o�C�g��ւ̃|�C���^
	*	@param offset	���l�̃I�t�Z�b�g
	*	@param size		���l�̃o�C�g��(1�`4)
	*
	*	@return �����������l
	*/
	uint32_t Get(const uint8_t* p, size_t offset, size_t size);

	/**
	* �o�b�t�@����DDS�摜����ǂݏo��.
	*
	* @param buf �ǂݏo�����o�b�t�@.
	*
	* @return �ǂݏo����DDS�摜���.
	*/
	DDSPixelFormat ReadDDSPixelFormat(const uint8_t* buf);

	/**
	* �o�b�t�@����DDS�t�@�C���w�b�_��ǂݏo��.
	*
	* @param buf �ǂݏo�����o�b�t�@.
	*
	* @return �ǂݏo����DDS�t�@�C���w�b�_.
	*/
	DDSHeader ReadDDSHeader(const uint8_t* buf);

	/**
	* DDS�t�@�C������e�N�X�`�����쐬����.
	*
	* @param filename DDS�t�@�C����.
	* @param st       DDS�t�@�C���X�e�[�^�X.
	* @param buf      �t�@�C����ǂݍ��񂾃o�b�t�@.
	* @param header   DDS�w�b�_�i�[��ւ̃|�C���^.
	*
	* @retval 0�ȊO �쐬�����e�N�X�`��ID.
	* @retval 0     �쐬���s.
	*/
	GLuint LoadDDS(const char* filename, const struct stat& st,
		const uint8_t* buf, DDSHeader* pHeader);
}