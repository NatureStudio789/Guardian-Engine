#ifndef _GE_GUARDIANSCRIPTCLASS_H_
#define _GE_GUARDIANSCRIPTCLASS_H_
#include "../ScriptRegistry/GuardianScriptRegistry.h"

namespace GE
{
	class GUARDIAN_API GuardianScriptClass
	{
	public:
		struct ScriptField
		{
		public:
			enum Category
			{
				GE_NONE = 0,
				GE_FLOAT, GE_DOUBLE, 
				GE_BOOL, GE_CHAR, GE_BYTE, GE_SHORT, GE_INT, GE_LONG, 
				GE_UBYTE, GE_USHORT, GE_UINT, GE_ULONG,
				GE_GVECTOR2, GE_GVECTOR3, GE_GVECTOR4,
				GE_ENTITY
			};

		public:
			ScriptField()
			{
				this->DataBuffer = malloc(16);
				if (this->DataBuffer)
				{
					memset(this->DataBuffer, 0, 16);
				}
			}

			template<typename T>
			T GetValue()
			{
				return *(T*)this->DataBuffer;
			}
#define EXPORT_GET_VALUE_FUNCTION(Type) template Type GUARDIAN_API GuardianScriptClass::ScriptField::GetValue<Type>();

			void* GetValueBuffer()
			{
				return this->DataBuffer;
			}

			template<typename T>
			void SetValue(T data)
			{
				memcpy(this->DataBuffer, &data, sizeof(T));
			}
#define EXPORT_SET_VALUE_FUNCTION(Type) template void GUARDIAN_API GuardianScriptClass::ScriptField::SetValue<Type>(Type data);

			const GString GetFieldStringType() const
			{
				return FieldTypeToString(this->Type);
			}

			static const GString FieldTypeToString(Category type)
			{
				switch (type)
				{
				case GE_NONE: return "None"; break;
				case GE_FLOAT: return "Float"; break;
				case GE_DOUBLE: return "Double"; break;
				case GE_BOOL: return "Bool"; break;
				case GE_CHAR: return "Char"; break;
				case GE_BYTE: return "Byte"; break;
				case GE_SHORT: return "Short"; break;
				case GE_INT: return "Int"; break;
				case GE_LONG: return "Long"; break;
				case GE_UBYTE: return "UByte"; break;
				case GE_USHORT: return "UShort"; break;
				case GE_UINT: return "UInt"; break;
				case GE_ULONG: return "ULong"; break;
				case GE_GVECTOR2: return "GVector2"; break;
				case GE_GVECTOR3: return "GVector3"; break;
				case GE_GVECTOR4: return "GVector4"; break;
				case GE_ENTITY: return "Entity"; break;
				default: return "Unknown"; break;
				}
			}

			void SetFieldStringType(const GString& type)
			{
				this->Type = StringToFieldType(type);
			}

			static Category StringToFieldType(const GString& type)
			{
				if (type == "None" || type == "Unknown")
				{
					return GE_NONE;
				}
				else if (type == "Float")
				{
					return GE_FLOAT;
				}
				else if (type == "Double")
				{
					return GE_DOUBLE;
				}
				else if (type == "Bool")
				{
					return GE_BOOL;
				}
				else if (type == "Char")
				{
					return GE_CHAR;
				}
				else if (type == "Byte")
				{
					return GE_BYTE;
				}
				else if (type == "Short")
				{
					return GE_SHORT;
				}
				else if (type == "Int")
				{
					return GE_INT;
				}
				else if (type == "Long")
				{
					return GE_LONG;
				}
				else if (type == "UByte")
				{
					return GE_UBYTE;
				}
				else if (type == "UShort")
				{
					return GE_USHORT;
				}
				else if (type == "UInt")
				{
					return GE_UINT;
				}
				else if (type == "ULong")
				{
					return GE_ULONG;
				}
				else if (type == "GVector2")
				{
					return GE_GVECTOR2;
				}
				else if (type == "GVector3")
				{
					return GE_GVECTOR3;
				}
				else if (type == "GVector4")
				{
					return GE_GVECTOR4;
				}
				else if (type == "Entity")
				{
					return GE_ENTITY;
				}
				else
				{
					return GE_NONE;
				}
			}

			MonoClassField* ClassField;

			GString Name;
			Category Type;

		private:
			void* DataBuffer;
		};

	public:
		GuardianScriptClass() = default;
		GuardianScriptClass(const GuardianScriptClass& other);
		GuardianScriptClass(const GString& classNamespace, const GString& className);

		void InitializeScriptClass(const GString classNamespace, const GString& className);
		void InstantiateObject();

		void InvokeMethod(MonoMethod* method, void** parameters);
		void InvokeMethod(const GString& name, int parameterCount, void** parameters);

		MonoObject* GetClassObject();
		MonoMethod* GetClassMethod(const GString& name, int parameterCount);

		bool HasField(const GString& name) const;
		ScriptField& GetClassField(const GString& name);
		void SetFieldValue(const GString& name, void* value);
		void* GetFieldValue(const GString& name);
		std::map<GString, ScriptField> GetClassFieldList();

	private:
		GString ClassNamespace;
		GString ClassName;

		std::map<GString, ScriptField> ClassFieldList;

		MonoClass* ClassInstance = null;
		MonoObject* ClassObject = null;

		friend class GuardianScriptEngine;
	};

	EXPORT_SET_VALUE_FUNCTION(float)
	EXPORT_GET_VALUE_FUNCTION(float)
	EXPORT_SET_VALUE_FUNCTION(int)
	EXPORT_GET_VALUE_FUNCTION(int)
}

#endif