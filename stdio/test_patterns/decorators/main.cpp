/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:18:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 21:31:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BasicPrinter.hpp"
#include "UppercaseDecorator.hpp"
#include "TimeStampDecorator.hpp"
#include "EmojiDecorator.hpp"
#include <memory>

int main()
{
	std::unique_ptr<Printer> printer =
		std::make_unique<UppercaseDecorator>(
			std::make_unique<TimeStampDecorator>(
				std::make_unique<EmojiDecorator>(
					std::make_unique<BasicPrinter>())));
	printer->print("hello world");
}

/**
 * 😎 [14:51:08] HELLO WORLD! 😎
 */