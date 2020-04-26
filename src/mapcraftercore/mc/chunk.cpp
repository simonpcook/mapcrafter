/*
 * Copyright 2012-2016 Moritz Hilscher
 *
 * This file is part of Mapcrafter.
 *
 * Mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "chunk.h"

#include "blockstate.h"

#include <cmath>
#include <iostream>

namespace mapcrafter {
namespace mc {

namespace {

uint64_t readBits(uint64_t Data, unsigned from, unsigned to) {
	Data >>= from;
	uint64_t Mask = (1ULL << (to-from+1)) - 1;
	return Data & Mask;
}

unsigned dataSizetoBitSize(size_t size) {
	// The "packed" version is was data.size() * 64 / (16*16*16)
	// So this the size will store 64 versions of the data
	switch (size) {
		default: assert(false && "Unsupported size");
		/* NEW SIZES */
		case 64: return 1;
		case 128: return 2;
		case 196: return 3;
		case 256: return 4;
		case 342: return 5;
		case 410: return 6;
		case 456: return 7;
		case 512: return 8;
		case 586: return 9;
		case 683: return 10;
		case 820: return 11; /* FIXME: COULD BE 12 TOO! */

		/* OLD SIZES */
		case 192: return 3;
		case 320: return 5;
		case 384: return 6;
		case 448: return 7;
		case 576: return 9;
		case 640: return 10;
		case 704: return 11;
		case 768: return 12;
	}
}

bool isNewSize(size_t size) {
	switch (size) {
		default: return false;
		case 64:
		case 128:
		case 196:
		case 256:
		case 342:
		case 410:
		case 456:
		case 512:
		case 586:
		case 683:
		case 820:
			return true;
	}
}

void readPackedShortsNew(const std::vector<int64_t>& data, uint16_t* palette) {
	int bits_per_entry = dataSizetoBitSize(data.size());

	unsigned int i = 0, j = 0;
	while (i < data.size()) {
		switch(bits_per_entry) {
			default: assert(false && "unknown bits_per_entry");
			case 4:
				if (j<4096) palette[j++] = readBits(data[i], 0, 3);
				if (j<4096) palette[j++] = readBits(data[i], 4, 7);
				if (j<4096) palette[j++] = readBits(data[i], 8, 11);
				if (j<4096) palette[j++] = readBits(data[i], 12, 15);
				if (j<4096) palette[j++] = readBits(data[i], 16, 19);
				if (j<4096) palette[j++] = readBits(data[i], 20, 23);
				if (j<4096) palette[j++] = readBits(data[i], 24, 27);
				if (j<4096) palette[j++] = readBits(data[i], 28, 31);
				if (j<4096) palette[j++] = readBits(data[i], 32, 35);
				if (j<4096) palette[j++] = readBits(data[i], 36, 39);
				if (j<4096) palette[j++] = readBits(data[i], 40, 43);
				if (j<4096) palette[j++] = readBits(data[i], 44, 47);
				if (j<4096) palette[j++] = readBits(data[i], 48, 51);
				if (j<4096) palette[j++] = readBits(data[i], 52, 55);
				if (j<4096) palette[j++] = readBits(data[i], 56, 59);
				if (j<4096) palette[j++] = readBits(data[i], 60, 63);
				break;
			case 5:
				if (j<4096) palette[j++] = readBits(data[i], 0, 4);
				if (j<4096) palette[j++] = readBits(data[i], 5, 9);
				if (j<4096) palette[j++] = readBits(data[i], 10, 14);
				if (j<4096) palette[j++] = readBits(data[i], 15, 19);
				if (j<4096) palette[j++] = readBits(data[i], 20, 24);
				if (j<4096) palette[j++] = readBits(data[i], 25, 29);
				if (j<4096) palette[j++] = readBits(data[i], 30, 34);
				if (j<4096) palette[j++] = readBits(data[i], 35, 39);
				if (j<4096) palette[j++] = readBits(data[i], 40, 44);
				if (j<4096) palette[j++] = readBits(data[i], 45, 49);
				if (j<4096) palette[j++] = readBits(data[i], 50, 54);
				if (j<4096) palette[j++] = readBits(data[i], 55, 59);
				break;
			case 6:
				if (j<4096) palette[j++] = readBits(data[i], 0, 5);
				if (j<4096) palette[j++] = readBits(data[i], 6, 11);
				if (j<4096) palette[j++] = readBits(data[i], 12, 17);
				if (j<4096) palette[j++] = readBits(data[i], 18, 23);
				if (j<4096) palette[j++] = readBits(data[i], 24, 29);
				if (j<4096) palette[j++] = readBits(data[i], 30, 35);
				if (j<4096) palette[j++] = readBits(data[i], 36, 41);
				if (j<4096) palette[j++] = readBits(data[i], 42, 47);
				if (j<4096) palette[j++] = readBits(data[i], 48, 53);
				if (j<4096) palette[j++] = readBits(data[i], 54, 59);
				break;
			case 7:
				if (j<4096) palette[j++] = readBits(data[i], 0, 6);
				if (j<4096) palette[j++] = readBits(data[i], 7, 13);
				if (j<4096) palette[j++] = readBits(data[i], 14, 20);
				if (j<4096) palette[j++] = readBits(data[i], 21, 27);
				if (j<4096) palette[j++] = readBits(data[i], 28, 34);
				if (j<4096) palette[j++] = readBits(data[i], 35, 41);
				if (j<4096) palette[j++] = readBits(data[i], 42, 48);
				if (j<4096) palette[j++] = readBits(data[i], 49, 55);
				if (j<4096) palette[j++] = readBits(data[i], 56, 62);
				break;
			case 8:
				if (j<4096) palette[j++] = readBits(data[i], 0, 7);
				if (j<4096) palette[j++] = readBits(data[i], 8, 15);
				if (j<4096) palette[j++] = readBits(data[i], 16, 23);
				if (j<4096) palette[j++] = readBits(data[i], 24, 31);
				if (j<4096) palette[j++] = readBits(data[i], 32, 39);
				if (j<4096) palette[j++] = readBits(data[i], 40, 47);
				if (j<4096) palette[j++] = readBits(data[i], 48, 55);
				if (j<4096) palette[j++] = readBits(data[i], 56, 63);
				break;
			case 9:
				if (j<4096) palette[j++] = readBits(data[i], 0, 8);
				if (j<4096) palette[j++] = readBits(data[i], 9, 17);
				if (j<4096) palette[j++] = readBits(data[i], 18, 26);
				if (j<4096) palette[j++] = readBits(data[i], 27, 35);
				if (j<4096) palette[j++] = readBits(data[i], 36, 44);
				if (j<4096) palette[j++] = readBits(data[i], 45, 53);
				if (j<4096) palette[j++] = readBits(data[i], 54, 62);
				break;
			case 10:
				if (j<4096) palette[j++] = readBits(data[i], 0, 9);
				if (j<4096) palette[j++] = readBits(data[i], 10, 19);
				if (j<4096) palette[j++] = readBits(data[i], 20, 29);
				if (j<4096) palette[j++] = readBits(data[i], 30, 39);
				if (j<4096) palette[j++] = readBits(data[i], 40, 49);
				if (j<4096) palette[j++] = readBits(data[i], 50, 59);
				break;
			case 11:
				if (j<4096) palette[j++] = readBits(data[i], 0, 10);
				if (j<4096) palette[j++] = readBits(data[i], 11, 21);
				if (j<4096) palette[j++] = readBits(data[i], 22, 32);
				if (j<4096) palette[j++] = readBits(data[i], 33, 43);
				if (j<4096) palette[j++] = readBits(data[i], 44, 54);
		}
		i++;
	}
	assert(j == 16*16*16);
}

void readPackedShorts(const std::vector<int64_t>& data, uint16_t* palette) {
	// If this a post-20w17a format data block, use new read code
	if (isNewSize(data.size()))
		return readPackedShortsNew(data, palette);

	// this is basically taken from Minecraft Overviewer
	// https://github.com/gmcnew/Minecraft-Overviewer/blob/minecraft113/overviewer_core/world.py#L809
	// maybe we can keep this, or maybe we could do something with unions for speed-up?
	const uint8_t* b = reinterpret_cast<const uint8_t*>(&data.front());
	int bits_per_entry = data.size() * 64 / (16*16*16);

	unsigned int i = 0, j = 0;
	while (i < data.size() * sizeof(int64_t)) {
		if (bits_per_entry == 4) {
			for (int k = 0; k < 4; k++) {
				palette[j++] = b[i+k] & 0x0f;
				palette[j++] = (b[i+k] & 0xf0) >> 4;
			}
			i += 4;
		}
		if (bits_per_entry == 5) {
			palette[j++] =   b[i] & 0x1f;
			palette[j++] = ((b[i+1] & 0x03) << 3) | ((b[i] & 0xe0) >> 5);
			palette[j++] =  (b[i+1] & 0x7c) >> 2;
			palette[j++] = ((b[i+2] & 0x0f) << 1) | ((b[i+1] & 0x80) >> 7);
			palette[j++] = ((b[i+3] & 0x01) << 4) | ((b[i+2] & 0xf0) >> 4);
			palette[j++] =  (b[i+3] & 0x3e) >> 1;
			palette[j++] = ((b[i+4]   & 0x07) << 2) | ((b[i+3] & 0xc0) >> 6);
			palette[j++] =  (b[i+4]   & 0xf8) >> 3;
			i += 5;
		}
		if (bits_per_entry == 6) {
			palette[j++] =  b[i] & 0x3f;
			palette[j++] = ((b[i+1] & 0x0f) << 2) | ((b[i]   & 0xc0) >> 6);
			palette[j++] = ((b[i+2] & 0x03) << 4) | ((b[i+1] & 0xf0) >> 4);
			palette[j++] =  (b[i+2] & 0xfc) >> 2;
			i += 3;
		}
		if (bits_per_entry == 7) {
			palette[j++] = b[i] & 0x7f;
			palette[j++] = ((b[i+1] & 0x3f) << 1) | ((b[i]   & 0x80) >> 7);
			palette[j++] = ((b[i+2] & 0x1f) << 2) | ((b[i+1] & 0xc0) >> 6);
			palette[j++] = ((b[i+3] & 0x0f) << 3) | ((b[i+2] & 0xe0) >> 5);
			palette[j++] = ((b[i+4] & 0x07) << 4) | ((b[i+3] & 0xf0) >> 4);
			palette[j++] = ((b[i+5] & 0x03) << 5) | ((b[i+4] & 0xf8) >> 3);
			palette[j++] = ((b[i+6] & 0x01) << 6) | ((b[i+5] & 0xfc) >> 2);
			palette[j++] =  (b[i+6] & 0xfe) >> 1;
			i += 7;
		}
		if (bits_per_entry == 8) {
			palette[j++] = b[i];
			i += 1;
		}
		if (bits_per_entry == 9) {
			palette[j++] = ((b[i+1] & 0x01) << 8) | b[0];
			palette[j++] = ((b[i+2] & 0x03) << 7) | ((b[i+1] & 0xfe) >> 1);
			palette[j++] = ((b[i+3] & 0x07) << 6) | ((b[i+2] & 0xfc) >> 2);
			palette[j++] = ((b[i+4] & 0x0f) << 5) | ((b[i+3] & 0xf8) >> 3);
			palette[j++] = ((b[i+5] & 0x1f) << 4) | ((b[i+4] & 0xf0) >> 4);
			palette[j++] = ((b[i+6] & 0x3f) << 3) | ((b[i+5] & 0xe0) >> 5);
			palette[j++] = ((b[i+7] & 0x7f) << 2) | ((b[i+6] & 0xc0) >> 6);
			palette[j++] =  (b[i+8] << 1) | ((b[i+7] & 0x80) >> 7);
			i += 9;
		}
		if (bits_per_entry == 10) {
			palette[j++] = ((b[i+1] & 0x03) << 8) |   b[0];
			palette[j++] = ((b[i+2] & 0x0f) << 6) | ((b[i+1] & 0xfc) >> 2);
			palette[j++] = ((b[i+3] & 0x3f) << 4) | ((b[i+2] & 0xf0) >> 4);
			palette[j++] =  (b[i+4] << 2)         | ((b[i+3] & 0xc0) >> 6);
			i += 5;
		}
		if (bits_per_entry == 11) {
			palette[j++] = ((b[i+1] & 0x07) << 8) |   b[0];
			palette[j++] = ((b[i+2] & 0x3f) << 5) | ((b[i+1] & 0xf8) >> 3);
			palette[j++] = ((b[i+4] & 0x01) << 10)| (b[i+3] << 2) | ((b[i+2] & 0xc0) >> 6);
			palette[j++] = ((b[i+5] & 0x0f) << 7) | ((b[i+4] & 0xfe) >> 1);
			palette[j++] = ((b[i+6] & 0x7f) << 4) | ((b[i+5] & 0xf0) >> 4);
			palette[j++] = ((b[i+8] & 0x03) << 9) | (b[i+7] << 1) | ((b[i+6] & 0x80) >> 7);
			palette[j++] = ((b[i+9] & 0x1f) << 2) | ((b[i+8] & 0xfc) >> 2);
			palette[j++] =  (b[i+10]        << 3) | ((b[i+9] & 0xe0) >> 5);
			i += 11;
		}
		if (bits_per_entry == 12) {
			palette[j++] = ((b[i+1] & 0x0f) << 8) |   b[0];
			palette[j++] =  (b[i+2]         << 4) | ((b[i+1] & 0xf0) >> 4);
			i += 3;
		}
	}
	assert(j == 16*16*16);
}

}

Chunk::Chunk()
	: chunkpos(42, 42), rotation(0), air_id(0) {
	clear();
}

Chunk::~Chunk() {
}

void Chunk::setRotation(int rotation) {
	this->rotation = rotation;
}

void Chunk::setWorldCrop(const WorldCrop& world_crop) {
	this->world_crop = world_crop;
}

int Chunk::positionToKey(int x, int z, int y) const {
	return y + 256 * (x + 16 * z);
}

bool Chunk::readNBT(mc::BlockStateRegistry& block_registry, const char* data, size_t len,
		nbt::Compression compression) {
	clear();

	air_id = block_registry.getBlockID(mc::BlockState("minecraft:air"));

	nbt::NBTFile nbt;
	nbt.readNBT(data, len, compression);

	// find "level" tag
	if (!nbt.hasTag<nbt::TagCompound>("Level")) {
		LOG(ERROR) << "Corrupt chunk: No level tag found!";
		return false;
	}
	const nbt::TagCompound& level = nbt.findTag<nbt::TagCompound>("Level");

	// then find x/z pos of the chunk
	if (!level.hasTag<nbt::TagInt>("xPos") || !level.hasTag<nbt::TagInt>("zPos")) {
		LOG(ERROR) << "Corrupt chunk: No x/z position found!";
		return false;
	}
	chunkpos_original = ChunkPos(level.findTag<nbt::TagInt>("xPos").payload,
	                             level.findTag<nbt::TagInt>("zPos").payload);
	chunkpos = chunkpos_original;
	if (rotation)
		chunkpos.rotate(rotation);

	// now we have the original chunk position:
	// check whether this chunk is completely contained within the cropped world
	chunk_completely_contained = world_crop.isChunkCompletelyContained(chunkpos_original);

	if (level.hasTag<nbt::TagString>("Status")) {
		const nbt::TagString& tag = level.findTag<nbt::TagString>("Status");
		// completely generated chunks in fresh 1.13 worlds usually have status 'fullchunk' or 'postprocessed'
		// however, chunks of converted <1.13 worlds don't use these, but the state 'mobs_spawned'
		if (!(tag.payload == "fullchunk" || tag.payload == "full" || tag.payload == "postprocessed" || tag.payload == "mobs_spawned")) {
			return true;
		}
	}

	if (level.hasArray<nbt::TagByteArray>("Biomes", BIOMES_ARRAY_SIZE)) {
		const nbt::TagByteArray& biomes_tag = level.findTag<nbt::TagByteArray>("Biomes");
		std::copy(biomes_tag.payload.begin(), biomes_tag.payload.end(), biomes);
	} else if (level.hasArray<nbt::TagIntArray>("Biomes", BIOMES_ARRAY_SIZE)) {
		const nbt::TagIntArray& biomes_tag = level.findTag<nbt::TagIntArray>("Biomes");
		std::copy(biomes_tag.payload.begin(), biomes_tag.payload.end(), biomes);
	} else if (level.hasArray<nbt::TagByteArray>("Biomes", 0)
			|| level.hasArray<nbt::TagLongArray>("Biomes", 0)) {
		std::fill(biomes, biomes + BIOMES_ARRAY_SIZE, 0);
	} else if (level.hasArray<nbt::TagByteArray>("Biomes", 256) || level.hasArray<nbt::TagIntArray>("Biomes", 256)) {
		LOG(WARNING) << "Out dated chunk " << chunkpos << ": Old biome data found!";
	} else {
		LOG(WARNING) << "Corrupt chunk " << chunkpos << ": No biome data found!";
		//level.dump(std::cout);
	}

	// find sections list
	// ignore it if section list does not exist, can happen sometimes with the empty
	// chunks of the end
	if (!level.hasList<nbt::TagCompound>("Sections"))
		return true;
	
	const nbt::TagList& sections_tag = level.findTag<nbt::TagList>("Sections");
	if (sections_tag.tag_type != nbt::TagCompound::TAG_TYPE)
		return true;

	// go through all sections
	for (auto it = sections_tag.payload.begin(); it != sections_tag.payload.end(); ++it) {
		const nbt::TagCompound& section_tag = (*it)->cast<nbt::TagCompound>();
		
		// make sure section is valid
		if (!section_tag.hasTag<nbt::TagByte>("Y")
		//		|| !section_tag.hasArray<nbt::TagByteArray>("Blocks", 4096)
		//		|| !section_tag.hasArray<nbt::TagByteArray>("Data", 2048)
				|| !section_tag.hasArray<nbt::TagLongArray>("BlockStates")
				|| !section_tag.hasTag<nbt::TagList>("Palette"))
			continue;
		
		const nbt::TagByte& y = section_tag.findTag<nbt::TagByte>("Y");
		if (y.payload >= CHUNK_HEIGHT)
			continue;
		//const nbt::TagByteArray& blocks = section_tag.findTag<nbt::TagByteArray>("Blocks");
		//const nbt::TagByteArray& data = section_tag.findTag<nbt::TagByteArray>("Data");
	
		const nbt::TagLongArray& blockstates = section_tag.findTag<nbt::TagLongArray>("BlockStates");
		
		const nbt::TagList& palette = section_tag.findTag<nbt::TagList>("Palette");
		std::vector<mc::BlockState> palette_blockstates(palette.payload.size());
		std::vector<uint16_t> palette_lookup(palette.payload.size());

		size_t i = 0;
		for (auto it2 = palette.payload.begin(); it2 != palette.payload.end(); ++it2, ++i) {
			const nbt::TagCompound& entry = (*it2)->cast<nbt::TagCompound>();
			const nbt::TagString& name = entry.findTag<nbt::TagString>("Name");
			
			mc::BlockState block(name.payload);
			if (entry.hasTag<nbt::TagCompound>("Properties")) {
				const nbt::TagCompound& properties = entry.findTag<nbt::TagCompound>("Properties");
				for (auto it3 = properties.payload.begin(); it3 != properties.payload.end(); ++it3) {
					std::string key = it3->first;
					std::string value = it3->second->cast<nbt::TagString>().payload;
					if (block_registry.isKnownProperty(block.getName(), key)) {
						block.setProperty(key, value);
					}
				}
			}
			palette_blockstates[i] = block;
			palette_lookup[i] = block_registry.getBlockID(block);
		}

		// create a ChunkSection-object
		ChunkSection section;
		section.y = y.payload;

		readPackedShorts(blockstates.payload, section.block_ids);
		
		int bits_per_entry = dataSizetoBitSize(blockstates.payload.size());
		bool ok = true;
		for (size_t i = 0; i < 16*16*16; i++) {
			if (section.block_ids[i] >= palette_blockstates.size()) {
				LOG(ERROR) << "Incorrectly parsed palette ID " << section.block_ids[i]
					<< " at index " << i << " (max is " << palette_blockstates.size()-1
					<< " with " << bits_per_entry << " bits per entry)";
				ok = false;
				break;
			}
			section.block_ids[i] = palette_lookup[section.block_ids[i]];
		}
		if (!ok) {
			continue;
		}

		if (section_tag.hasArray<nbt::TagByteArray>("BlockLight", 2048)) {
			const nbt::TagByteArray& block_light = section_tag.findTag<nbt::TagByteArray>("BlockLight");
			std::copy(block_light.payload.begin(), block_light.payload.end(), section.block_light);
		} else {
			std::fill(&section.block_light[0], &section.block_light[2048], 0);
		}
		if (section_tag.hasArray<nbt::TagByteArray>("SkyLight", 2048)) {
			const nbt::TagByteArray& sky_light = section_tag.findTag<nbt::TagByteArray>("SkyLight");
			std::copy(sky_light.payload.begin(), sky_light.payload.end(), section.sky_light);
		} else {
			std::fill(&section.sky_light[0], &section.sky_light[2048], 0);
		}

		// add this section to the section list
		section_offsets[section.y] = sections.size();
		sections.push_back(section);
	}

	return true;
}

void Chunk::clear() {
	sections.clear();
	for (int i = 0; i < CHUNK_HEIGHT; i++)
		section_offsets[i] = -1;
	std::fill(biomes, biomes + 256, 21 /* DEFAULT_BIOME */);
}

bool Chunk::hasSection(int section) const {
	return section < CHUNK_HEIGHT && section_offsets[section] != -1;
}

void rotateBlockPos(int& x, int& z, int rotation) {
	int nx = x, nz = z;
	for (int i = 0; i < rotation; i++) {
		nx = z;
		nz = 15 - x;
		x = nx;
		z = nz;
	}
}

uint16_t Chunk::getBlockID(const LocalBlockPos& pos, bool force) const {
	// at first find out the section and check if it's valid and contained
	int section = pos.y / 16;
	if (section >= CHUNK_HEIGHT || section_offsets[section] == -1)
		return air_id;
	// FIXME sometimes this happens, fix this
	//if (sections.size() > 16 || sections.size() <= (unsigned) section_offsets[section]) {
	//	return 0;
	//}

	// if rotated: rotate position to position with original rotation
	int x = pos.x;
	int z = pos.z;
	if (rotation)
		rotateBlockPos(x, z, rotation);

	// check whether this block is really rendered
	if (!checkBlockWorldCrop(x, z, pos.y))
		return air_id;

	// calculate the offset and get the block ID
	// and don't forget the add data
	int offset = ((pos.y % 16) * 16 + z) * 16 + x;
	uint16_t id = sections[section_offsets[section]].block_ids[offset];
	if (!force && world_crop.hasBlockMask()) {
		const BlockMask* mask = world_crop.getBlockMask();
		BlockMask::BlockState block_state = mask->getBlockState(id);
		if (block_state == BlockMask::BlockState::COMPLETELY_HIDDEN)
			return air_id;
		else if (block_state == BlockMask::BlockState::COMPLETELY_SHOWN)
			return id;
		if (mask->isHidden(id, 0 /*getBlockData(pos, true)*/))
			return air_id;
	}
	return id;
}

bool Chunk::checkBlockWorldCrop(int x, int z, int y) const {
	// now about the actual world cropping:
	// get the global position of the block, with the original world rotation
	BlockPos global_pos = LocalBlockPos(x, z, y).toGlobalPos(chunkpos_original);
	// check whether the block is contained in the y-bounds.
	if (!world_crop.isBlockContainedY(global_pos))
		return false;
	// only check x/z-bounds if the chunk is not completely contained
	if (!chunk_completely_contained && !world_crop.isBlockContainedXZ(global_pos))
		return false;
	return true;
}

uint8_t Chunk::getData(const LocalBlockPos& pos, int array, bool force) const {
	// at first find out the section and check if it's valid and contained
	int section = pos.y / 16;
	if (section >= CHUNK_HEIGHT || section_offsets[section] == -1) {
		 // not existing sections should always have skylight
		 return array == 1 ? 15 : 0;
	}

	// if rotated: rotate position to position with original rotation
	int x = pos.x;
	int z = pos.z;
	if (rotation)
		 rotateBlockPos(x, z, rotation);

	// check whether this block is really rendered
	if (!checkBlockWorldCrop(x, z, pos.y)) {
		 return array == 1 ? 15 : 0;
	}

	uint8_t data = 0;
	// calculate the offset and get the block data
	int offset = ((pos.y % 16) * 16 + z) * 16 + x;
	// handle bottom/top nibble
	if ((offset % 2) == 0)
		 data = sections[section_offsets[section]].getArray(array)[offset / 2] & 0xf;
	else
		 data = (sections[section_offsets[section]].getArray(array)[offset / 2] >> 4) & 0x0f;
	if (!force && world_crop.hasBlockMask()) {
		 const BlockMask* mask = world_crop.getBlockMask();
		 if (mask->isHidden(getBlockID(pos, true), data)) {
			  return array == 1 ? 15 : 0;
		}
	}
	return data;
}

uint8_t Chunk::getBlockLight(const LocalBlockPos& pos) const {
	return getData(pos, 0);
}

uint8_t Chunk::getSkyLight(const LocalBlockPos& pos) const {
	return getData(pos, 1);
}

uint8_t Chunk::getBiomeAt(const LocalBlockPos& pos) const {
	int x = pos.x / 4;
	int z = pos.z / 4;
	int y = pos.y / 4;

	if (rotation)
		rotateBlockPos(x, z, rotation);

	return biomes[(y * 16 + (z * 4 + x))];
}

const ChunkPos& Chunk::getPos() const {
	return chunkpos;
}

}
}
