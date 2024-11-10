import { Heading } from "@radix-ui/themes";
import { Shield } from "lucide-react";
import Link from "next/link";

function Navbar() {
  return (
    <div className="w-screen flex items-center justify-between shadow-lg ring-1 ring-black/5 bg-slate-800/40 rounded-xl">
      <Link href={"/"}>
        <div className="flex items-center p-8 gap-2">
          <Shield className="w-8 h-8" />

          <Heading as="h1" size={"7"} weight={"medium"}>
            Secure Proxy
          </Heading>
        </div>
      </Link>

      <div className="flex items-center gap-9 p-8 uppercase">
        {/* <button className="uppercase rounded-3xl text-xl px-7 bg-purple-700 py-1">BUY</button> */}
      </div>
    </div>
  );
}

export default Navbar;
