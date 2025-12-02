/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimeStampDecorator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:23:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:31:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "PrinterDecorator.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

class TimeStampDecorator : public PrinterDecorator
{
public:
    using PrinterDecorator::PrinterDecorator;

    void print(const std::string &msg) override
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "[" << std::put_time(std::localtime(&t), "%H:%M:%S") << "] " << msg;
        wrapped->print(ss.str());
    }
};