/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Printer.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:59:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:12:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

class Printer {
public:
    virtual ~Printer() = default;
    virtual void print(const std::string& msg) = 0;
};
