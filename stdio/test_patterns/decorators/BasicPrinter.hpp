/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicPrinter.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:29:20 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:33:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "PrinterDecorator.hpp"
#include <iostream>
#include <string>

class BasicPrinter : public Printer
{
public:
    void print(const std::string &msg) override
    {
        std::cout << msg << std::endl;
    }
};
