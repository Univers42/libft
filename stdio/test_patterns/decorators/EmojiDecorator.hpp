/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmojiDecorator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:21:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:31:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "PrinterDecorator.hpp"
#include <string>

class EmojiDecorator : public PrinterDecorator
{
public:
    using PrinterDecorator::PrinterDecorator;
    void print(const std::string &msg) override
    {
        wrapped->print("😎 " + msg + " 😎");
    }
};