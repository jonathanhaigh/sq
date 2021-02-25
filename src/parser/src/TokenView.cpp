/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/TokenView.h"

#include "common_types/LexError.h"
#include "util/ASSERT.h"
#include "util/typeutil.h"

namespace sq::parser {

namespace {

using util::to_size_t;
using util::to_ptrdiff_t;

const auto& token_regex_map()
{
    static const auto map = std::array{
        std::pair{Token::Kind::BoolTrue, std::regex{"true"}},
        std::pair{Token::Kind::BoolFalse, std::regex{"false"}},
        std::pair{Token::Kind::LParen, std::regex{"\\("}},
        std::pair{Token::Kind::RParen, std::regex{"\\)"}},
        std::pair{Token::Kind::LBrace, std::regex{"\\{"}},
        std::pair{Token::Kind::RBrace, std::regex{"\\}"}},
        std::pair{Token::Kind::LBracket, std::regex{"\\["}},
        std::pair{Token::Kind::RBracket, std::regex{"\\]"}},
        std::pair{Token::Kind::Dot, std::regex{"\\."}},
        std::pair{Token::Kind::Equals, std::regex{"="}},
        std::pair{Token::Kind::Comma, std::regex{","}},
        std::pair{Token::Kind::Colon, std::regex{":"}},
        std::pair{Token::Kind::Identifier, std::regex{"[A-Za-z_][A-Za-z_0-9]*"}},
        std::pair{Token::Kind::DQString, std::regex{R"%("([^"]|\\")*")%"}},
        std::pair{Token::Kind::Integer, std::regex{"-?[0-9]+"}}
    };
    return map;
}

} // namespace

TokenView::TokenView(std::string_view str) noexcept
    : str_{str}
{ }

const Token& TokenView::read() const
{
    ASSERT(pos_ != -1);

    if (cache_)
    {
        return cache_.value();
    }

    const auto pos = pos_ + whitespace_length();
    const auto remaining = str_.substr(to_size_t(pos));

    if (std::ssize(remaining) == 0)
    {
        cache_ = Token(str_, pos, 0, Token::Kind::Eof);
        return cache_.value();
    }

    for (const auto& [token_kind, regex] : token_regex_map())
    {
        
        if (auto match = std::match_results<std::string_view::const_iterator>{};
            std::regex_search(
                remaining.begin(), remaining.end(),
                match,
                regex,
                std::regex_constants::match_continuous))
        {
            cache_ = Token(str_, pos, match.length(), token_kind);
            return cache_.value();
        }
    }

    throw LexError{pos, str_};
}

void TokenView::next()
{
    if (!cache_)
    {
        (void)read();
        ASSERT(cache_);
    }
    if (cache_.value().kind() == Token::Kind::Eof)
    {
        pos_ = -1;
        cache_ = std::nullopt;
        return;
    }
    pos_ = cache_.value().pos() + cache_.value().len();
    cache_ = std::nullopt;
}

std::ptrdiff_t TokenView::whitespace_length() const
{
    const auto remaining = str_.substr(to_size_t(pos_));
    return to_ptrdiff_t(std::min(
        remaining.find_first_not_of(" \t\r\n"),
        remaining.size()
    ));
}

bool TokenView::equal(
    [[maybe_unused]] ranges::default_sentinel_t other
) const noexcept
{
    return pos_ == -1;
}

} // namespace sq::parser
