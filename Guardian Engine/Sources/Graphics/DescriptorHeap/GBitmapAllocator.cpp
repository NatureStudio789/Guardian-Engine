#include "GBitmapAllocator.h"

#define __builtin_popcountll(x) __popcnt((UINT)x)

namespace GE
{
	GBitmapAllocator::GBitmapAllocator()
	{
		this->DescriptorCount = 0;
		this->Bitmap.clear();
	}

	GBitmapAllocator::GBitmapAllocator(ULONGLONG count)
	{
		this->InitializeBitmapAllocator(count);
	}

	GBitmapAllocator::GBitmapAllocator(const GBitmapAllocator& other)
	{
		this->DescriptorCount = other.DescriptorCount;
		this->Bitmap = other.Bitmap;
	}

	GBitmapAllocator::~GBitmapAllocator()
	{
		this->DescriptorCount = 0;

		this->Bitmap.clear();
	}

	void GBitmapAllocator::InitializeBitmapAllocator(ULONGLONG count)
	{
		GUARDIAN_CHECK_VALUE(count);

		this->DescriptorCount = count;
		this->Bitmap.resize((this->DescriptorCount + 63) / 64, 0);
	}

	UINT GBitmapAllocator::Allocate(ULONGLONG count)
	{
		GUARDIAN_CHECK_VALUE(count);
		
		if (this->DescriptorCount < count)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No more room for allocation!");
		}

		ULONGLONG CurrentFreeCount = 0;
		ULONGLONG StartIndex = 0;

		for (ULONGLONG i = 0; i < this->DescriptorCount; i++)
		{
			if (!this->IsBitSet(i))
			{
				CurrentFreeCount++;

				if (CurrentFreeCount == count)
				{
					this->MarkAllocated(StartIndex, i);

					return (UINT)StartIndex;
				}
			}
			else
			{
				CurrentFreeCount = 0;
				StartIndex = i + 1;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("Can't found enough size for allocation!");
	}

	void GBitmapAllocator::Free(ULONGLONG start, ULONGLONG count)
	{
		if (start >= this->DescriptorCount || count == 0 || (start + count) > this->DescriptorCount)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The free range is invalid!");
		}

		for (ULONGLONG i = start; i < start + count; i++)
		{
			this->ClearBit(i);
		}
	}

	const ULONGLONG& GBitmapAllocator::GetDescriptorCount() const noexcept
	{
		return this->DescriptorCount;
	}

	const ULONGLONG GBitmapAllocator::GetAllocatedCount() const
	{
		ULONGLONG count = 0;
		for (const auto& block : this->Bitmap)
		{
			count += __builtin_popcountll(block);
		}

		return count;
	}

	const ULONGLONG GBitmapAllocator::GetFreeCount() const
	{
		return this->DescriptorCount - this->GetAllocatedCount();
	}

	bool GBitmapAllocator::IsBitSet(ULONGLONG index) const
	{
		ULONGLONG BlockIndex = index / 64;
		ULONGLONG BitOffset = index % 64;

		return (this->Bitmap[BlockIndex] & (1ULL << BitOffset)) != 0;
	}

	void GBitmapAllocator::SetBit(ULONGLONG index)
	{
		ULONGLONG BlockIndex = index / 64;
		ULONGLONG BitOffset = index % 64;

		this->Bitmap[BlockIndex] |= (1ULL << BitOffset);
	}

	void GBitmapAllocator::ClearBit(ULONGLONG index)
	{
		ULONGLONG BlockIndex = index / 64;
		ULONGLONG BitOffset = index % 64;

		this->Bitmap[BlockIndex] &= ~(1ULL << BitOffset);
	}

	void GBitmapAllocator::MarkAllocated(ULONGLONG start, ULONGLONG end)
	{
		for (ULONGLONG i = start; i <= end; i++)
		{
			this->SetBit(i);
		}
	}
}
