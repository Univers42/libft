/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:44:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 13:52:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_HPP
# define TESTER_HPP

// When we're building a shared library /.so/.dll, not everything inside it is
// automatically visible to other programs. 
/**
 * to reinforce private and public visibility
 * TESTER_API_ makrs the  public ones.
 * Our tester will be shipped as a shared library henceforth, we must
 * expose certain classes:
 * testing::test
 * testing::Message
 * testing::String
 * test registration internals
 * assertion helpers
 * 
 * This as for consequences to let the compiler understand that if they're 
 * not declared with this attribute, then the compiler may:
 * - hide them
 * - strip them
 * - rename them intentionally
 * - make linking fails
 */
# if defined(_WIN32)
#  define TESTER_API_ __delspec(dllexport)
# elif defined(__GNUC__)    /* window*/
#  define TESTER_API_ __attribute__((visibility("default")))
# else  /* UNIX */
#  define TESTER_API
# endif /* NOTHING */

#endif