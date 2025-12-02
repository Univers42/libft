/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:06:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:12:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ProxyPrinter.hpp"
#include "RealPrinter.hpp"

int main() {
    std::unique_ptr<Printer> real = std::make_unique<RealPrinter>();
    ProxyPrinter proxy(std::move(real));

    proxy.print("Hello world!");

    proxy.setEnabled(false);
    proxy.print("This should be blocked!");

    return 0;
}
