#ifndef _GE_GBITMAPALLOCATOR_H_
#define _GE_GBITMAPALLOCATOR_H_
#include "../SwapChain/GSwapChain.h"

namespace GE
{
	class GUARDIAN_API GBitmapAllocator
	{
	public:
		GBitmapAllocator();
		GBitmapAllocator(ULONGLONG count);
		GBitmapAllocator(const GBitmapAllocator& other);
		~GBitmapAllocator();

		void InitializeBitmapAllocator(ULONGLONG count);

		UINT Allocate(ULONGLONG count);
		void Free(ULONGLONG start, ULONGLONG count);

		const ULONGLONG& GetDescriptorCount() const noexcept;
		const ULONGLONG GetAllocatedCount() const;
		const ULONGLONG GetFreeCount() const;

		static std::shared_ptr<GBitmapAllocator> CreateNewBitmapAllocator(ULONGLONG count)
		{
			return std::make_shared<GBitmapAllocator>(count);
		}

	private:
		bool IsBitSet(ULONGLONG index) const;
		void SetBit(ULONGLONG index);
		void ClearBit(ULONGLONG index);
		void MarkAllocated(ULONGLONG start, ULONGLONG end);

		ULONGLONG DescriptorCount;
		std::vector<ULONGLONG> Bitmap;
	};
}

#endif