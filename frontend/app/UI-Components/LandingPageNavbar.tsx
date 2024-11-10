import { Heading, Text } from "@radix-ui/themes";
import { Shield } from "lucide-react";
import Link from "next/link";

function LandingPageNavbar() {
  return (
    <div className="w-screen flex items-center justify-between shadow-xl ring-1 ring-black/5 bg-black/10 sticky top-0 bg-gradient-to-br from-violet-950 via-violet-700 to-violet-900">
      <Link href={"/"}>
        <div className="flex items-center p-8 gap-2">
          <Shield className="w-8 h-8" />
          <Heading as="h1" size={"7"} weight={"medium"}>
            Secure Proxy
          </Heading>
        </div>
      </Link>

      <div className="flex items-center gap-9 p-8 uppercase">
        <Link href={"/"}>
          <Text size={"4"} weight={"medium"}>
            Home
          </Text>
        </Link>
        <Link href={"/about"}>
          <Text size={"4"} weight={"medium"}>
            About
          </Text>
        </Link>
        <button className="uppercase rounded-3xl text-xl px-7 bg-purple-700 py-1">Testing Playground</button>
      </div>
    </div>
  );
}

export default LandingPageNavbar;
