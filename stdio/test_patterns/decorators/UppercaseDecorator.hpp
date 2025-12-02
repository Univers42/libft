/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UppercaseDecorator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:26:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:31:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "PrinterDecorator.hpp"
#include <algorithm>
#include <cctype>

class UppercaseDecorator : public PrinterDecorator
{
public:
    using PrinterDecorator::PrinterDecorator;
    void print(const std::string &msg) override
    {
        std::string up = msg;
        std::transform(up.begin(), up.end(), up.begin(),
                       [](unsigned char c)
                       { return static_cast<char>(std::toupper(c)); });
        wrapped->print(up);
    }
};