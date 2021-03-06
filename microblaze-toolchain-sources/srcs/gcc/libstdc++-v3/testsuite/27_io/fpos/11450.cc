// Copyright (C) 2003
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// 27.4.3 fpos

#include <ios>
#include <testsuite_hooks.h>

// libstdc++/11450
void test01()
{
  bool test __attribute__((unused)) = true;  

  std::fpos<int> p;
  VERIFY( p.state() == 0 );
  p.state(42);
  VERIFY( p.state() == 42 );

  std::fpos<int> q = p + static_cast<std::streamoff>(1);
  VERIFY( q.state() == 42 );
}

int main()
{
  test01();
  return 0;
}
