/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RealPrinter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:01:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:12:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Printer.hpp"
#include <iostream>

class RealPrinter : public Printer {
public:
    void print(const std::string& msg) override {
        std::cout << "[REAL] " << msg << std::endl;
    }
};
