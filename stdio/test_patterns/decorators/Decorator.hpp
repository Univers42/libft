/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Decorator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:29:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:31:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "PrinterDecorator.hpp"
#include <memory>

class PrinterDecorator : public Printer
{
protected:
    std::unique_ptr<Printer> wrapped;

public:
    PrinterDecorator(std::unique_ptr<Printer> printer)
        : wrapped(std::move(printer)) {}
};
