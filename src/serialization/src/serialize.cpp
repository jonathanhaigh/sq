#include "serialization/serialize.h"

#include "results/results.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <gsl/narrow>
#include <string>

namespace rj = rapidjson;

namespace sq::serialization {

template <typename Writer>
class RjPrimitiveVisitor
{
public:
    explicit RjPrimitiveVisitor(Writer& writer)
        : writer_{&writer}
    { }

    void operator()(const field_types::PrimitiveString& str)
    {
        // RapidJSON's String's size type can be smaller than
        // std::string::size_type so we have to narrow here.
        writer_->String(
            str.c_str(),
            gsl::narrow<rj::SizeType>(str.size()),
            true /* copy */
        );
    }

    void operator()(const field_types::PrimitiveInt& i)
    {
        writer_->Int64(i);
    }

    void operator()(const field_types::PrimitiveBool& b)
    {
        writer_->Bool(b);
    }

    void operator()(const field_types::PrimitiveFloat& f)
    {
        writer_->Double(f);
    }

private:
    Writer* writer_;
};

template <typename Writer>
class RjResultVisitor
{
public:
    explicit RjResultVisitor(Writer& writer)
        : writer_{&writer}
    { }

    void operator()(const results::ResultTree::ObjData& obj)
    {
        writer_->StartObject();
        for (const auto& [key, child] : obj)
        {
            writer_->Key(key.c_str());
            std::visit(*this, child.data());
        }
        writer_->EndObject();
    }

    void operator()(const results::ResultTree::ArrayData& arr)
    {
        writer_->StartArray();
        for (const auto& child : arr)
        {
            std::visit(*this, child.data());
        }
        writer_->EndArray();
    }

    void operator()(const results::ResultTree::PrimitiveData& prim)
    {
        std::visit(RjPrimitiveVisitor{*writer_}, prim); 
    }

private:
    Writer* writer_; 
};

void serialize_results(std::ostream& os, const results::ResultTree& results)
{
    assert(results.ast().data().name().empty());
    auto buf = rj::StringBuffer{};
    auto writer = rj::Writer<rj::StringBuffer>{buf};
    std::visit(RjResultVisitor{writer}, results.data());
    os << buf.GetString() << '\n';
}

} // namespace sq::serialization

