/* Aseprite
 * Copyright (C) 2001-2013  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/app.h"
#include "app/commands/command.h"
#include "app/context_access.h"
#include "app/ui/timeline.h"
#include "app/ui/main_window.h"
#include "ui/base.h"

namespace app {

class CopyCelCommand : public Command {
public:
  CopyCelCommand();
  Command* clone() const override { return new CopyCelCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

CopyCelCommand::CopyCelCommand()
  : Command("CopyCel",
            "Copy Cel",
            CmdUIOnlyFlag)
{
}

bool CopyCelCommand::onEnabled(Context* context)
{
  return App::instance()->getMainWindow()->getTimeline()->isMovingCel();
}

void CopyCelCommand::onExecute(Context* context)
{
  App::instance()->getMainWindow()->getTimeline()->dropRange(Timeline::kCopy);
}

Command* CommandFactory::createCopyCelCommand()
{
  return new CopyCelCommand;
}

} // namespace app