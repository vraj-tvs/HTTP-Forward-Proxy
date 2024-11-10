import { Card, CardContent } from "@/components/ui/card";

function DisplayCard({ image, title, description }: { image: JSX.Element; title: string; description: string }) {
  return (
    <Card className="bg-gradient-to-br from-indigo-900 via-violet-800 to-indigo-900 bg-opacity-50 border-black py-20 shadow-2xl mx-4">
      <CardContent className="flex flex-col items-center space-y-4 p-6">
        {image}
        <h3 className="text-xl font-semibold">{title}</h3>
        <p className="text-white">{description}</p>
      </CardContent>
    </Card>
  );
}

export default DisplayCard;
